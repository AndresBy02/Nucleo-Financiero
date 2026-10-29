# Bitácora de Decisiones de Arquitectura

| Fecha | Decisión | Alternativas descartadas | Por qué se descartaron | Requisito de calidad justificado |
| :--- | :--- | :--- | :--- | :--- |
| 2026-10-29 | Preasignación de arreglos de salida desde el llamador (Java reserva `double[]` y C solo llena) | Reserva dinámica con `malloc` dentro de C y función `fin_free()` | Evita fugas de memoria y travesías JNA adicionales para liberar punteros en bloques `try/finally` | RQ-01 (Desempeño) y simplicidad de contrato |
| 2026-10-29 | Regla de redondeo centralizada al peso (`round` a 0 decimales) en `fin_redondear()` | Redondear en cada periodo intermedio del VPN o truncar decimales | Redondear por periodo acumula error; el contrato exige cuadrar al peso con la hoja de control | RN-01, RN-03, RN-04 |
| 2026-10-29 | Carga de `libfinanzas.so` en `/usr/lib` mediante `LD_LIBRARY_PATH` y `java.library.path` en Dockerfile multietapa | Empaquetar el `.so` dentro del JAR o compilar con `gcc` en la imagen final | Mantener `gcc` en la imagen final viola el límite de 250 MB y la restricción de no incluir compilador | RQ-03 (Facilidad de despliegue) |
