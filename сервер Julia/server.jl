using DifferentialEquations, AMQPClient, JSON3, Symbolics, JSON

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
    elseif method_name == "rosenbrock23"
        return Rosenbrock23()
    elseif method_name == "rodas5"
        return Rodas5()
    elseif method_name == "implicit_euler"
        return ImplicitEuler()
    elseif method_name == "ken_carp4"
        return KenCarp4()
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

        # Преобразуем результат в JSON3
        result = Dict(
            "taskId" => task["taskId"],
            "solution" => [
                Dict("t" => t, "y" => y) for (t, y) in zip(sol.t, sol.u)
            ]
        )
        return JSON3.write(result; indent=4)
        # return Dict("taskId" => task["taskId"],
        # "solution" => [Dict("t" => t, "y" => y) for (t, y) in zip(sol.t, sol.u)])
    catch e
        return Dict("taskId" => task["taskId"], "error" => "Ошибка в вычислениях", "details" => string(e))
    end
end


json_input = """
{
  "equations": ["y1", "-100*y2"],
  "initial_conditions": [1.0, 1.0],
  "h0": 0.1,
  "t_start": 0.0,
  "t_end": 1.0,
  "accuracy": 1e-8,
  "method": "rodas5",
  "taskId": "1",
  "jacobi_matrix": [[1.0, 0.0], [0.0, -100.0]]
}
"""

task_dict = JSON.parse(json_input)

println(solve_ode(task_dict))

# 🔹 Подключаемся к RabbitMQ
# conn = AMQPConnection()
# channel = AMQPChannel(conn)
# queue_declare(channel, "juliaQueue")

# println("✅ Julia сервер запущен и слушает juliaQueue...")

# # 🔹 Обрабатываем задачи из `juliaQueue`
# consume(channel, "juliaQueue") do msg
#     try
#         task = JSON.parse(String(msg.body))
#         println("📩 Получена задача:", task)

#         result = solve_ode(task)
#         response = JSON.json(result)

#         println("📤 Отправляем результат в responseQueue")
#         basic_publish(channel, exchange="", routing_key="responseQueue", body=response)

#         basic_ack(msg)  # Подтверждаем выполнение
#     catch e
#         println("❌ Ошибка обработки задачи: ", e)
#     end
# end
