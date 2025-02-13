FROM julia:1.8

WORKDIR /app
COPY server.jl /app/server.jl
RUN julia -e 'using Pkg; Pkg.add(["DifferentialEquations", "AMQPClient", "JSON", "JSON3"])'

CMD ["julia", "/app/server.jl"]
