# Núcleo Financiero (`libfinanzas.so`)

Componente nativo en C para la aritmética financiera del presupuestador de obra (**Valor Presente Neto**, **Tasa Interna de Retorno** y **Tabla de Amortización**), integrado vía JNA en `calculo-service`.

## Estructura del Proyecto

```text
Nucleo_financiero/
├── native/                         # Entregable 01 y 03: Código fuente en C y suite de pruebas nativa
│   ├── include/
│   │   └── finanzas.h              # Cabecera oficial del contrato (firmas, códigos de error, regla de redondeo)
│   ├── src/
│   │   └── finanzas.c              # Implementación principal en C (VPN, TIR, Amortización y redondeo)
│   ├── tests/
│   │   └── test_finanzas.c         # Suite de pruebas en C (casos de referencia)
│   └── Makefile                    # Compilación de libfinanzas.so y ejecución de pruebas con un comando
├── calculo-service/                # Entregable 02 y 05: Integración JNA, línea base Java y Dockerfile
│   ├── src/main/java/co/edu/uceva/calculo/jna/
│   │   └── Finanzas.java           # Interfaz JNA mapeada contra libfinanzas.so
│   └── Dockerfile                  # Dockerfile multietapa (compila en gcc, ejecuta en JRE limpio)
└── docs/                           # Entregable 04 y 06: Documentación, mediciones y bitácora
    └── BITACORA.md                 # Registro de decisiones de arquitectura
```

## Comandos Rápidos (Entorno Linux / WSL / Docker)

### 1. Compilar la biblioteca compartida (`libfinanzas.so`)
```bash
cd native
make all
```

### 2. Compilar y ejecutar las pruebas en C (RQ-02)
```bash
cd native
make test
```
