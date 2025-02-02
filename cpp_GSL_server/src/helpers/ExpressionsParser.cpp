#include "ExpressionsParser.hpp"

void ExpressionsParser::convert_string_to_maths(std::shared_ptr<Task> task)
{
    table.add_constants();
    for (size_t i = 0; i < task->equations_strings.size(); ++i)
    {
        table.add_variable("y" + std::to_string(i), task->result_at_point[i]);
    }

    for (int i = 0; i < task->equations_strings.size(); i++)
    {
        exprtk::expression<double> expr;
        task->expressions.push_back(expr);
    }

    for (size_t i = 0; i < task->equations_strings.size(); ++i)
    {
        task->expressions[i].register_symbol_table(table);
        parser.compile(task->equations_strings[i], task->expressions[i]);
    }
}