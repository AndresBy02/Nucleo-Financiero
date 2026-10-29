package co.edu.uceva.calculo.jna;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.ptr.DoubleByReference;
import com.sun.jna.ptr.IntByReference;

/**
 * Contrato JNA con la biblioteca nativa libfinanzas.so
 */
public interface Finanzas extends Library {
    Finanzas INSTANCE = Native.load("finanzas", Finanzas.class);

    double fin_vpn(double tasa, double[] flujos, int n);

    int fin_tir(double[] flujos, int n, double tol, int maxIter,
                DoubleByReference tirOut, IntByReference iterOut);

    int fin_amortizacion(double capital, double tasaPeriodo, int periodos,
                         double[] cuotasOut);
}
