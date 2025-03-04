FROM julia:1.8

WORKDIR /app

# Устанавливаем системные зависимости (libev)
RUN apt-get update && apt-get install -y libev-dev

# Устанавливаем нужные пакеты сразу при сборке
RUN julia -e 'using Pkg; Pkg.add(["DifferentialEquations", "AMQPClient", "JSON", "Symbolics"])'
# RUN julia -e 'using Pkg; Pkg.add(["DifferentialEquations", "AMQPClient", "JSON", "Symbolics", Libc"])'
ENV JULIA_NUM_THREADS=4

# Копируем код сервера
COPY server.jl /app/

CMD ["julia", "/app/server.jl"]
