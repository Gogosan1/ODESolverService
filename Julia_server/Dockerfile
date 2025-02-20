FROM julia:1.8

WORKDIR /app

# Устанавливаем нужные пакеты сразу при сборке
RUN julia -e 'using Pkg; Pkg.add(["DifferentialEquations", "AMQPClient", "JSON", "JSON3", "Symbolics"])'

# Копируем код сервера
COPY server.jl /app/

CMD ["julia", "/app/server.jl"]
