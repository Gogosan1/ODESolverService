using DifferentialEquations, AMQPClient, Symbolics, JSON, MacroTools #, Libc

function get_solver(method_name)
    #Явные методы Рунге-Кутты в таком же порядке как в документации 
    if method_name == "Euler"
        return Euler()
    elseif method_name == "Midpoint"
        return Midpoint()
    elseif method_name == "Heun"
        return Heun()
    elseif method_name == "Ralston"
        return Ralston()
    elseif method_name == "RK4"
        return RK4()
    elseif method_name == "BS3"
        return BS3()
    elseif method_name == "OwrenZen3"
        return OwrenZen3()
    elseif method_name == "OwrenZen4"
        return OwrenZen4()
    elseif method_name == "OwrenZen5"
        return OwrenZen5()
    elseif method_name == "DP5"
        return DP5()
    elseif method_name == "RKO65"
        return RKO65()
    elseif method_name == "TanYam7"
        return TanYam7()
    elseif method_name == "DP8"
        return DP8()
    elseif method_name == "TsitPap8"
        return TsitPap8()
    elseif method_name == "TsitPap10"
        return TsitPap10()
    elseif method_name == "Feagin12"
        return Feagin12()
    elseif method_name == "Feagin14"
        return Feagin14()
    elseif method_name == "MSRK5"
        return MSRK5()
    elseif method_name == "MSRK6"
        return MSRK6()
    elseif method_name == "Stepanov5"
        return Stepanov5()
    elseif method_name == "SIR54"
        return SIR54()
    elseif method_name == "Alshina2"
        return Alshina2()
    elseif method_name == "Alshina3"
        return Alshina3()
    elseif method_name == "Alshina6"
        return Alshina6()
        # методы с ленивой интерполяцией
    elseif method_name == "BS5"
        return BS5()
    elseif method_name == "Vern6"
        return Vern6()
    elseif method_name == "Vern7"
        return Vern7()
    elseif method_name == "Vern8"
        return Vern8()
    elseif method_name == "Vern9"
        return Vern9()
        # Явные методы Адамса-Башфорта
    elseif method_name == "AB3"
        return AB3()
    elseif method_name == "AB4"
        return AB4()
    elseif method_name == "AB5"
        return AB5()
    elseif method_name == "ABM32"
        return ABM32()
    elseif method_name == "ABM43"
        return ABM43()
    elseif method_name == "ABM54"
        return ABM54()
        # Адаптивный размер шага Явные методы Адамса
    elseif method_name == "VCAB3"
        return VCAB3()
    elseif method_name == "VCAB4"
        return VCAB4()
    elseif method_name == "VCAB5"
        return VCAB5()
    elseif method_name == "VCABM3"
        return VCABM3()
    elseif method_name == "VCABM4"
        return VCABM4()
    elseif method_name == "VCABM5"
        return VCABM5()
    elseif method_name == "VCABM"
        return VCABM()
    elseif method_name == "AN5"
        return AN5()
    elseif method_name == "JVODE_Adams"
        return JVODE_Adams()
        #OrdinaryDiffEq.jl для жестких уравнений
        #Методы SDIRK
    elseif method_name == "ImplicitEuler"
        return ImplicitEuler()
    elseif method_name == "ImplicitMidpoint"
        return ImplicitMidpoint()
    elseif method_name == "Trapezoid"
        return Trapezoid()
    elseif method_name == "TRBDF2"
        return TRBDF2()
    elseif method_name == "SDIRK2"
        return SDIRK2()
    elseif method_name == "Kvaerno3"
        return Kvaerno3()
    elseif method_name == "KenCarp3"
        return KenCarp3()
    elseif method_name == "Cash4"
        return Cash4()
    elseif method_name == "Hairer4"
        return Hairer4()
    elseif method_name == "Hairer42"
        return Hairer42()
    elseif method_name == "Kvaerno4"
        return Kvaerno4()
    elseif method_name == "KenCarp4"
        return KenCarp4()
    elseif method_name == "KenCarp47"
        return KenCarp47()
    elseif method_name == "Kvaerno5"
        return Kvaerno5()
    elseif method_name == "KenCarp5"
        return KenCarp5()
    elseif method_name == "KenCarp58"
        return KenCarp58()
    elseif method_name == "ESDIRK54I8L2SA"
        return ESDIRK54I8L2SA()
    elseif method_name == "ESDIRK436L2SA2"
        return ESDIRK436L2SA2()
    elseif method_name == "ESDIRK437L2SA"
        return ESDIRK437L2SA()
    elseif method_name == "ESDIRK547L2SA2"
        return ESDIRK547L2SA2()
        #Полностью неявные методы Рунге-Кутты (FIRK)
    elseif method_name == "RadauIIA3"
        return RadauIIA3()
    elseif method_name == "RadauIIA5"
        return RadauIIA5()
        #Методы Розенброка
    elseif method_name == "ROS3P"
        return ROS3P()
    elseif method_name == "Rodas3"
        return Rodas3()
    elseif method_name == "Rodas3P"
        return Rodas3P()
    elseif method_name == "RosShamp4"
        return RosShamp4()
    elseif method_name == "Veldd4"
        return Veldd4()
    elseif method_name == "Velds4"
        return Velds4()
    elseif method_name == "GRK4T"
        return GRK4T()
    elseif method_name == "GRK4A"
        return GRK4A()
    elseif method_name == "Ros4LStab"
        return Ros4LStab()
    elseif method_name == "Rodas4"
        return Rodas4()
    elseif method_name == "Rodas42"
        return Rodas42()
    elseif method_name == "Rodas4P"
        return Rodas4P()
    elseif method_name == "Rodas4P2"
        return Rodas4P2()
    elseif method_name == "Rodas5"
        return Rodas5()
    elseif method_name == "Rodas5P"
        return Rodas5P()
    elseif method_name == "ROS2"
        return ROS2()
    elseif method_name == "ROS3"
        return ROS3()
    elseif method_name == "ROS2PR"
        return ROS2PR()
    elseif method_name == "ROS3PR"
        return ROS3PR()
    elseif method_name == "Scholz4_7"
        return Scholz4_7()
    elseif method_name == "ROS3PRL"
        return ROS3PRL()
    elseif method_name == "ROS3PRL2"
        return ROS3PRL2()
        #Методы Розенброка-В.
    elseif method_name == "Rosenbrock23"
        return Rosenbrock23()
    elseif method_name == "Rosenbrock32"
        return Rosenbrock32()
    elseif method_name == "Rodas23W"
        return Rodas23W()
    elseif method_name == "RosenbrockW6S4OS"
        return RosenbrockW6S4OS()
    elseif method_name == "ROS34PW1a"
        return ROS34PW1a()
    elseif method_name == "ROS34PW1b"
        return ROS34PW1b()
    elseif method_name == "ROS34PW2"
        return ROS34PW2()
    elseif method_name == "ROS34PW3"
        return ROS34PW3()
    elseif method_name == "ROS34PRw"
        return ROS34PRw()
    elseif method_name == "ROS2S"
        return ROS2S()
        #Стабилизированные явные методы
    elseif method_name == "ROCK2"
        return ROCK2()
    elseif method_name == "ROCK4"
        return ROCK4()
    elseif method_name == "RKC"
        return RKC()
    elseif method_name == "SERK2"
        return SERK2()
    elseif method_name == "ESERK5"
        return ESERK5()
        #Многошаговые методы
    elseif method_name == "QNDF1"
        return QNDF1()
    elseif method_name == "QBDF1"
        return QBDF1()
    elseif method_name == "ABDF2"
        return ABDF2()
    elseif method_name == "QNDF2"
        return QNDF2()
    elseif method_name == "QBDF2"
        return QBDF2()
    elseif method_name == "QNDF"
        return QNDF()
    elseif method_name == "QBDF"
        return QBDF()
    elseif method_name == "MEBDF2"
        return MEBDF2()
    elseif method_name == "FBDF"
        return FBDF()
        #метод по умолчанию
        return Tsit5()
    end
end

function substitute_vars_to_function(expr::Expr, vars::Dict{String,Int})
    substituted_expr = MacroTools.postwalk(expr) do x
        if x isa Symbol && haskey(vars, string(x))
            idx = vars[string(x)]
            return :(u[$idx])
        else
            return x
        end
    end

    # Оборачиваем в min(..., 0.0), чтобы получить числовую функцию с нулем в нужный момент
    wrapped_expr = :(min($substituted_expr, 0.0))
    return eval(:(u -> $wrapped_expr))
end

function substitute_vars_to_affect(expr::Expr, vars::Dict{String,Int})
    substituted_expr = MacroTools.postwalk(expr) do x
        if x isa Symbol && haskey(vars, string(x))
            idx = vars[string(x)]
            return :(integrator.u[$idx])
        else
            return x
        end
    end

    return eval(:(integrator -> $substituted_expr))
end



function build_callbacks(events_json::Vector, num_vars::Int)
    vars_dict = Dict("y$(i)" => i for i in 1:num_vars)
    callbacks = []

    for ev in events_json
        condition_expr = Meta.parse(ev["condition"])
        affect_expr = Meta.parse(ev["affect"])

        cond_func = substitute_vars_to_function(condition_expr, vars_dict)
        affect_func = substitute_vars_to_affect(affect_expr, vars_dict)

        condition = (u, t, integrator) -> begin
        # Выводим информацию о текущем значении y1 и проверке условия
        println("Step: t = $t, y1 = $(u[1]), Condition: $(Base.invokelatest(cond_func, u))")
        return Base.invokelatest(cond_func, u)
    end

    affect! = integrator -> begin
        # Выводим информацию о срабатывании события
        println("Event triggered, changing y2 and y1")
        Base.invokelatest(affect_func, integrator)
    end
        # condition = (u, t, integrator) -> Base.invokelatest(cond_func, u)
        # affect! = integrator -> Base.invokelatest(affect_func, integrator)

        cb = DiscreteCallback(condition, affect!, save_positions=(true, true))
        push!(callbacks, cb)
    end

    return CallbackSet(callbacks...)
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

        callbacks = haskey(task, "events") ? build_callbacks(task["events"], length(y0)) : nothing

        prob = ODEProblem(f, y0, tspan)
        sol = callbacks === nothing ?
              solve(prob, method; dt=h0, saveat=h0) : #reltol=accuracy,
              solve(prob, method; dt=h0, saveat=h0, callback=callbacks) #reltol=accuracy,

        #sol = solve(prob, method, reltol=accuracy, dt=h0)

        result = Dict(
            "sessionId" => task["sessionId"],
            "solution" => [
                Dict("t" => t, "y" => y) for (t, y) in zip(sol.t, sol.u)
            ]
        )
        return JSON.json(result)

    catch e
        return JSON.json(Dict("sessionId" => task["sessionId"], "error" => "Ошибка в вычислениях", "details" => string(e)))
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


