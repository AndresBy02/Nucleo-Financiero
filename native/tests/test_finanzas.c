#include <stdio.h>
#include <math.h>
#include "finanzas.h"

int main(void) {
    printf("=== Suite de pruebas base: libfinanzas ===\n\n");

    /* Caso 1: VPN con tasa 0% (suma simple) */
    double flujos[] = {-1000000.0, 300000.0, 400000.0, 500000.0};
    double vpn_cero = fin_vpn(0.0, flujos, 4);
    printf("[VPN Tasa 0%%] Esperado: 200000 | Obtenido: %.0f\n", vpn_cero);

    /* Caso 2: TIR basica */
    double tir = 0.0;
    int iter = 0;
    int rc_tir = fin_tir(flujos, 4, 1e-6, 100, &tir, &iter);
    printf("[TIR] Codigo: %d | Tasa: %.6f | Iteraciones: %d\n", rc_tir, tir, iter);

    /* Caso 3: Tabla de amortizacion */
    double cuotas[3 * 4];
    int rc_amort = fin_amortizacion(1000000.0, 0.02, 3, cuotas);
    printf("[Amortizacion] Codigo: %d | Saldo final: %.0f\n", rc_amort, cuotas[2 * 4 + 3]);

    return 0;
}
