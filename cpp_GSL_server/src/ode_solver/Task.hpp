#pragma once
#include <vector>
#include <string>
#include "../exprtk/exprtk.hpp"
struct Task
{
    // TODO::Add lambdas for find jacobian
    std::vector<std::string> equations_strings;
    std::vector<std::vector<std::string>> jacobi_matrix_string;
    std::vector<exprtk::expression<double>> expressions;
    std::vector<double> start_conditions; // WARNING!!! This argument is changed by the solver!
    double h0;
    double t_start;
    double t_end;
    double accuracy;
};