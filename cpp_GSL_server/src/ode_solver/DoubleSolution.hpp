#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
class Solution
{
private:
    std::vector<std::pair<double, std::vector<double>>> results; // Поддержка int и double
    int dimension;
    std::vector<exprtk::expression<double>> expressions;
    std::vector<std::vector<exprtk::expression<double>>> jackobi_matrix_expressions;

public:
    void add_result_at_point(double point, std::vector<double> results);
    Solution(int dimension);
    std::vector<std::pair<double, std::vector<double>>> get_results();
};
