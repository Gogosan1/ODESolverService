#pragma once
#include <vector>
#include <memory>
#include "../ode_solver/Task.hpp"
#include "../exprtk/exprtk.hpp"

class ExpressionsParser
{
private:
    exprtk::symbol_table<double> table;
    exprtk::parser<double> parser;

public:
    void convert_string_to_maths(std::shared_ptr<Task> task);
};