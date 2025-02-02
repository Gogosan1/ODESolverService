#include "JSONHelper.hpp"

void JSONHelper::upload_from_json(nlohmann::json file, std::shared_ptr<Task> task, Method *method)
{
    // TODO::Add lambdas convert / add validation
    task->equations_strings = file["equations"].get<std::vector<std::string>>();
    task->result_at_point = file["result_at_start_point"].get<std::vector<double>>();
    task->h0 = file["h0"].get<double>();
    task->t_start = file["t_start"].get<double>();
    task->t_end = file["t_end"].get<double>();
    task->accuracy = file["accuracy"].get<double>();

    std::string string_method = file["method"].get<std::string>();
    *method = stringToMethod(string_method);
}

Method JSONHelper::stringToMethod(const std::string &method_str)
{
    static const std::unordered_map<std::string, Method> method_map = {
        {"rk2", Method::rk2},
        {"rk4", Method::rk4},
        {"rkf45", Method::rkf45},
        {"rkck", Method::rkck},
        {"rk8pd", Method::rk8pd},
        {"rk1imp", Method::rk1imp},
        {"rk2imp", Method::rk2imp},
        {"rk4imp", Method::rk4imp},
        {"bsimp", Method::bsimp},
        {"msadams", Method::msadams},
        {"msbdf", Method::msbdf}};

    auto it = method_map.find(method_str);
    if (it != method_map.end())
    {
        return it->second;
    }
    else
    {
        return Method::unknown; // Возвращаем Unknown, если строка не распознана
    }
}