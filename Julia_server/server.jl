using DifferentialEquations, AMQPClient, Symbolics, JSON #, Libc

# 🔹 Функция выбора метода по строке
function get_solver(method_name)
    if method_name == "euler"
        return Euler()
    elseif method_name == "rk4"
        return RK4()
    elseif method_name == "dp5"
        return DP5()
    elseif method_name == "vern7"
        return Vern7()
    elseif method_name == "vern9"
        return Vern9()
    elseif method_name == "rosenbrock23"
        return Rosenbrock23()
    elseif method_name == "rodas5"
        return Rodas5()
    elseif method_name == "rodas4"
        return Rodas4()
    elseif method_name == "trbdf2"
        return TRBDF2()
    elseif method_name == "implicit_euler"
        return ImplicitEuler()
    elseif method_name == "ken_carp4"
        return KenCarp4()
    elseif method_name == "ken_carp5"
        return KenCarp5()
    elseif method_name == "ken_carp10"
        return KenCarp10()
    elseif method_name == "radau_iia5"
        return RadauIIA5()
    elseif method_name == "ab4"
        return AB4()
    elseif method_name == "bs3"
        return BS3()
    elseif method_name == "qn_df"
        return QNDF()
    elseif method_name == "kvaerno5"
        return Kvaerno5()
    elseif method_name == "rodas23"
        return Rodas23()
    elseif method_name == "fbdf"
        return FBDF()
    elseif method_name == "ida"
        return IDA()
    elseif method_name == "dassl"
        return DASSL()
    elseif method_name == "em"
        return EM()
    elseif method_name == "sriw1"
        return SRIW1()
    elseif method_name == "sra1"
        return SRA1()
    elseif method_name == "toms748"
        return TOMS748()
    elseif method_name == "simple_diff_eq"
        return SimpleDiffEq()
    else
        return Tsit5()  # 🚀 Дефолтный метод (универсальный)
    end
end

# 🔹 Функция для решения ОДУ
function solve_ode(task::Dict)
    try
        y0 = Float64.(task["initial_conditions"])
        tspan = (task["t_start"], task["t_end"])
        h0 = task["h0"]
        accuracy = task["accuracy"]

        # Выбираем численный метод
        method = get_solver(task["method"])

        function f(dy, y, p, t)
            equations = task["equations"]

            # Создаём символические переменные и подстановочный словарь в одном цикле
            substitution_dict = Dict()
            for i in eachindex(y)
                var = Symbolics.variable(Symbol("y$i"))
                substitution_dict[var] = y[i]
            end

            for (i, eq) in enumerate(equations)
                # Парсим уравнение из строки
                parsed_eq = Symbolics.parse_expr_to_symbolic(Meta.parse(eq), Main)

                # Подставляем значения в уравнение
                substituted = Symbolics.substitute(parsed_eq, substitution_dict)

                # Преобразуем символическое выражение в число
                evaluated = Symbolics.value(substituted)

                dy[i] = evaluated
            end
        end


        prob = ODEProblem(f, y0, tspan)

        sol = solve(prob, method, reltol=accuracy, dt=h0)

        # return Dict(
        #     "taskId" => task["taskId"],
        #     "solution" => [Dict("t" => t, "y" => y) for (t, y) in zip(sol.t, sol.u)]
        # )
        result = Dict(
            "taskId" => task["taskId"],
            "solution" => [
                Dict("t" => t, "y" => y) for (t, y) in zip(sol.t, sol.u)
            ]
        )
        return JSON.json(result)

    catch e
        return Dict("taskId" => task["taskId"], "error" => "Ошибка в вычислениях", "details" => string(e))
    end
end

# 🔹 Подключаемся к RabbitMQ
port = parse(Int, ENV["RABBITMQ_PORT"])#AMQPClient.AMQP_DEFAULT_PORT
login = ENV["RABBITMQ_USERNAME"]  # Логин, по умолчанию "guest"
password = ENV["RABBITMQ_PASSWORD"]  # Пароль, по умолчанию "guest"
REQUEST_QUEUE = ENV["JULIA_QUEUE"]
RESPONSE_QUEUE = ENV["RESPONSE_QUEUE"]
host = ENV["RABBITMQ_HOST"]
println(host)

auth_params = Dict{String,Any}("MECHANISM" => "AMQPLAIN", "LOGIN" => login, "PASSWORD" => password)

# Создание подключения к RabbitMQ
conn = connection(; virtualhost="/", host=host, port=port, auth_params=auth_params)

channel1 = channel(conn, AMQPClient.UNUSED_CHANNEL, true)

AMQPClient.queue_declare(channel1, REQUEST_QUEUE, durable=true)
AMQPClient.queue_declare(channel1, RESPONSE_QUEUE, durable=true)


println("✅ Julia сервер запущен и слушает juliaQueue...")

function process_message(msg)
    try
        if msg !== nothing
            task = JSON.parse(String(msg.data))
            println("📩 Получена задача: ", task)

            # Решаем систему ОДУ
            result = solve_ode(task)

            # Отправляем результат
            println("📤 Отправляем результат в responseQueue")
            data = convert(Vector{UInt8}, codeunits(result))
            msg1 = Message(data)
            basic_publish(channel1, msg1; routing_key=RESPONSE_QUEUE)

            # Подтверждаем обработку
            basic_ack(channel1, msg.delivery_tag)
            #else
            #println("⏳ Ожидание сообщений...")
        end
    catch e
        println("⚠ Ошибка обработки сообщения: ", e)
    end
end

# Бесконечный цикл обработки сообщений
basic_consume(channel1, REQUEST_QUEUE, process_message)

while true
    sleep(0.5)
end

# json_input = """
# {
#   "equations": ["y1", "-100*y2"],
#   "initial_conditions": [1.0, 1.0],
#   "h0": 0.1,
#   "t_start": 0.0,
#   "t_end": 1.0,
#   "accuracy": 1e-8,
#   "method": "rodas5",
#   "taskId": "1",
#   "jacobi_matrix": [[1.0, 0.0], [0.0, -100.0]]
# }
# """
