#ifndef FINANZAS_H
#define FINANZAS_H

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * RN-04: Regla de redondeo declarada en un solo lugar.
 * Cambiar el numero de decimales exige tocar unicamente esta linea.
 * ============================================================================ */
#define FIN_DECIMALES_REDONDEO 0

/* ============================================================================
 * RN-05: Codigos de retorno completos (Contrato con el frente de Web 2)
 * ============================================================================ */
#define FIN_OK                    0   /* Operacion exitosa / Convergencia alcanzada */
#define FIN_ERR_PARAM_INVALIDO   -1   /* Punteros nulos o parametros generales invalidos */
#define FIN_ERR_NO_CONVERGE      -2   /* La TIR no convergio dentro de max_iter */
#define FIN_ERR_CAPITAL_INVALIDO -3   /* Capital menor o igual a cero */
#define FIN_ERR_PERIODOS_INVALIDO -4  /* Numero de periodos menor o igual a cero */
#define FIN_ERR_TASA_FUERA_RANGO -5   /* Tasa fuera de rango permitido (ej. <= -1.0) */
#define FIN_ERR_TIR_MULTIPLE     -6   /* Flujo con multiples cambios de signo o sin cambio */

/* ============================================================================
 * Firmas del contrato (Documento de referencia)
 * ============================================================================ */

/**
 * RN-01: Valor presente neto: formula cerrada, una sola pasada.
 */
double fin_vpn(double tasa, const double *flujos, int n);

/**
 * RN-02: Tasa interna de retorno por aproximaciones sucesivas.
 * Devuelve 0 (FIN_OK) si convergio, negativo si no. La tasa sale por
 * tir_out y el conteo de iteraciones por iter_out: el numero de
 * iteraciones ES parte del resultado, no un dato de depuracion.
 */
int fin_tir(const double *flujos, int n, double tol, int max_iter,
            double *tir_out, int *iter_out);

/**
 * RN-03: Tabla de amortizacion: llena el arreglo cuotas_out (ya reservado por
 * quien llama, de tamano periodos * 4) con 'periodos' filas de:
 * [cuota, interes, abono, saldo].
 */
int fin_amortizacion(double capital, double tasa_periodo, int periodos,
                     double *cuotas_out);

#ifdef __cplusplus
}
#endif

#endif /* FINANZAS_H */
