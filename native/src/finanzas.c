#include "../include/finanzas.h"
#include <math.h>
#include <stddef.h>

/* ============================================================================
 * RN-04: La regla de redondeo, en un solo lugar.
 * Redondeo simetrico al mitad hacia arriba (half-away-from-zero / llround)
 * aplicado según FIN_DECIMALES_REDONDEO (0 decimales = al peso).
 * ============================================================================ */
static double fin_redondear(double valor) {
    double factor = pow(10.0, FIN_DECIMALES_REDONDEO);
    return round(valor * factor) / factor;
}

/* Funcion interna para evaluar el VPN sin redondear (usada en iteraciones de TIR) */
static double vpn_interno(double tasa, const double *flujos, int n) {
    if (tasa == 0.0) {
        double suma = 0.0;
        for (int t = 0; t < n; t++) {
            suma += flujos[t];
        }
        return suma;
    }

    double vpn = 0.0;
    double factor_descuento = 1.0;
    double base = 1.0 + tasa;

    for (int t = 0; t < n; t++) {
        vpn += flujos[t] / factor_descuento;
        factor_descuento *= base;
    }
    return vpn;
}

/* ============================================================================
 * RN-01: Valor Presente Neto (VPN)
 * Formula cerrada en una sola pasada, sin acumular error de redondeo entre
 * periodos. Redondeo aplicado una unica vez al final.
 * ============================================================================ */
double fin_vpn(double tasa, const double *flujos, int n) {
    if (flujos == NULL || n <= 0 || tasa <= -1.0) {
        return NAN;
    }

    double resultado = vpn_interno(tasa, flujos, n);
    return fin_redondear(resultado);
}

/* ============================================================================
 * RN-02: Tasa Interna de Retorno (TIR)
 * Por aproximaciones sucesivas (metodo de la secante / Newton-Raphson).
 * ============================================================================ */
int fin_tir(const double *flujos, int n, double tol, int max_iter,
            double *tir_out, int *iter_out) {
    if (flujos == NULL || tir_out == NULL || iter_out == NULL) {
        return FIN_ERR_PARAM_INVALIDO;
    }
    if (n <= 1) {
        return FIN_ERR_PERIODOS_INVALIDO;
    }
    if (tol <= 0.0 || max_iter <= 0) {
        return FIN_ERR_PARAM_INVALIDO;
    }

    /* Verificar cambios de signo en los flujos */
    int cambios_signo = 0;
    int signo_anterior = 0;
    for (int i = 0; i < n; i++) {
        if (flujos[i] != 0.0) {
            int signo_actual = (flujos[i] > 0.0) ? 1 : -1;
            if (signo_anterior != 0 && signo_actual != signo_anterior) {
                cambios_signo++;
            }
            signo_anterior = signo_actual;
        }
    }

    if (cambios_signo == 0) {
        *iter_out = 0;
        return FIN_ERR_NO_CONVERGE;
    }
    if (cambios_signo > 1) {
        *iter_out = 0;
        return FIN_ERR_TIR_MULTIPLE;
    }

    /* Aproximaciones sucesivas (Newton-Raphson) */
    double r = 0.10; /* Semilla inicial: 10% */

    for (int iter = 1; iter <= max_iter; iter++) {
        double f_val = 0.0;
        double f_deriv = 0.0;
        double base = 1.0 + r;

        if (base <= 1e-9) {
            *iter_out = iter;
            return FIN_ERR_TASA_FUERA_RANGO;
        }

        double factor = 1.0;
        for (int t = 0; t < n; t++) {
            f_val += flujos[t] / factor;
            if (t > 0) {
                f_deriv -= (t * flujos[t]) / (factor * base);
            }
            factor *= base;
        }

        if (fabs(f_val) <= tol) {
            *tir_out = r;
            *iter_out = iter;
            return FIN_OK;
        }

        if (fabs(f_deriv) < 1e-12) {
            *iter_out = iter;
            return FIN_ERR_NO_CONVERGE;
        }

        double r_sig = r - (f_val / f_deriv);
        if (fabs(r_sig - r) <= tol) {
            *tir_out = r_sig;
            *iter_out = iter;
            return FIN_OK;
        }

        r = r_sig;
    }

    *iter_out = max_iter;
    return FIN_ERR_NO_CONVERGE;
}

/* ============================================================================
 * RN-03: Tabla de amortizacion (sistema frances de cuota fija)
 * Llena cuotas_out con [cuota, interes, abono, saldo] por cada periodo.
 * Absorbe el residuo de redondeo en el ultimo periodo para que saldo == 0.
 * ============================================================================ */
int fin_amortizacion(double capital, double tasa_periodo, int periodos,
                     double *cuotas_out) {
    if (cuotas_out == NULL) {
        return FIN_ERR_PARAM_INVALIDO;
    }
    if (capital <= 0.0) {
        return FIN_ERR_CAPITAL_INVALIDO;
    }
    if (periodos <= 0) {
        return FIN_ERR_PERIODOS_INVALIDO;
    }
    if (tasa_periodo < 0.0) {
        return FIN_ERR_TASA_FUERA_RANGO;
    }

    double cuota_fija;
    if (tasa_periodo == 0.0) {
        cuota_fija = fin_redondear(capital / periodos);
    } else {
        double factor = pow(1.0 + tasa_periodo, periodos);
        cuota_fija = fin_redondear(capital * (tasa_periodo * factor) / (factor - 1.0));
    }

    double saldo = fin_redondear(capital);

    for (int p = 0; p < periodos; p++) {
        double interes = fin_redondear(saldo * tasa_periodo);
        double cuota = cuota_fija;
        double abono = cuota - interes;

        /* En el ultimo periodo se absorbe cualquier residuo de redondeo */
        if (p == periodos - 1) {
            abono = saldo;
            cuota = abono + interes;
            saldo = 0.0;
        } else {
            saldo = saldo - abono;
        }

        int idx = p * 4;
        cuotas_out[idx + 0] = cuota;
        cuotas_out[idx + 1] = interes;
        cuotas_out[idx + 2] = abono;
        cuotas_out[idx + 3] = saldo;
    }

    return FIN_OK;
}
