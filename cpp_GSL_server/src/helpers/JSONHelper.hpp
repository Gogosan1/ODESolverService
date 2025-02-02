#pragma once
#include "../ode_solver/Task.hpp"
#include <nlohmann/json.hpp>
#include <memory>
#include <unordered_map>
#include <string>
#include "../ode_solver/EnumMethods.hpp"
class JSONHelper
{
private:
    Method stringToMethod(const std::string &method);

public:
    void upload_from_json(nlohmann::json, std::shared_ptr<Task>, Method* method);
};