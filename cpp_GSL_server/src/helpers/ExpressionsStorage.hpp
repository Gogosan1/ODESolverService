#pragma once
#include <vector>
#include <memory>
#include "../ode_solver/Task.hpp"
#include "../exprtk/exprtk.hpp"

class ExpressionsStorage
{
private:
    int dimension;
    void get_maths_expressions_from_strings(std::vector<std::string> str_expr, std::vector<exprtk::expression<double>> &expr);
    exprtk::symbol_table<double> table;
    exprtk::parser<double> parser;

public:
    std::vector<exprtk::expression<double>> expressions;
    std::vector<std::vector<exprtk::expression<double>>> jacobi_matrix_expressions;
    std::vector<double> result_at_point;
    ExpressionsStorage(std::shared_ptr<Task> task);
    int get_dimension();
};