# syntax=docker/dockerfile:1

# 1️⃣ Стадия сборки: скачивание зависимостей и упаковка .jar
FROM eclipse-temurin:21-jdk-jammy AS build

WORKDIR /app

# Копируем только файлы, нужные для скачивания зависимостей
COPY mvnw .
COPY .mvn .mvn
COPY pom.xml .

RUN chmod +x mvnw && ./mvnw dependency:go-offline -DskipTests

# Копируем исходный код после кеширования зависимостей
COPY src/ src/
RUN ./mvnw package -DskipTests && \
    cp target/*.jar app.jar

# 2️⃣ Финальный минимизированный образ: только JRE и готовый .jar
FROM eclipse-temurin:21-jre-jammy AS final

WORKDIR /app

ARG UID=10001
RUN adduser \
    --disabled-password \
    --gecos "" \
    --home "/nonexistent" \
    --shell "/sbin/nologin" \
    --no-create-home \
    --uid "${UID}" \
    appuser

USER appuser

# Копируем только скомпилированный JAR
COPY --from=build /app/app.jar .

EXPOSE 8080

ENTRYPOINT ["java", "-jar", "app.jar"]
