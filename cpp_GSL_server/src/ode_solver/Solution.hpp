#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
class Solution
{
private:
    std::string data;
    int dimension;

public:
    void add_result_at_point(double point, std::vector<double> results);
    Solution(int dimension);
    std::string get_data();

   friend void to_json(nlohmann::json &j, const Solution &sol);
   friend void from_json(const nlohmann::json &j, Solution &sol);
};
