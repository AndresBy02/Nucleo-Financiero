# Etapa 1: Compilacion de la biblioteca nativa en C (libfinanzas.so)
FROM gcc:13-bookworm AS native-builder
WORKDIR /build/native
COPY native/ .
RUN make all

# Etapa 2: Imagen final ligera sin compilador ni fuentes en C (RQ-03)
FROM eclipse-temurin:21-jre-alpine
WORKDIR /app

# Copiar unicamente el artefacto compilado .so
COPY --from=native-builder /build/native/build/libfinanzas.so /usr/lib/libfinanzas.so

# Configurar ruta de carga para JNA
ENV LD_LIBRARY_PATH=/usr/lib

# Aqui se copiara el JAR del microservicio calculo-service
# COPY calculo-service/target/calculo-service.jar /app/app.jar

EXPOSE 8080
CMD ["java", "-Djava.library.path=/usr/lib", "-jar", "/app/app.jar"]
