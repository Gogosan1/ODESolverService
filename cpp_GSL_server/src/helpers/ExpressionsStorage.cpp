#include "ExpressionsStorage.hpp"

ExpressionsStorage::ExpressionsStorage(Task* task)
{

    dimension = task->start_conditions.size();
    result_at_point = task->start_conditions;

    expressions.reserve(dimension);
    for (size_t i = 0; i < dimension; ++i)
    {
        exprtk::expression<double> expr;
        expressions.push_back(expr);
    }
    // Создаём jacobi_matrix_expressions
    jacobi_matrix_expressions.resize(dimension);
    for (size_t i = 0; i < dimension; ++i)
    {
        jacobi_matrix_expressions[i].reserve(dimension);
        for (size_t j = 0; j < dimension; ++j)
        {
            exprtk::expression<double> expr;
            jacobi_matrix_expressions[i].push_back(expr);
        }
    }

    get_maths_expressions_from_strings(task->equations_strings, expressions);

    if (task->jacobi_matrix_string.size() != 0)
    {
        for (int i = 0; i < dimension; i++)
        {
            get_maths_expressions_from_strings(task->jacobi_matrix_string[i], jacobi_matrix_expressions[i]);
        }
    }
}

int ExpressionsStorage::get_dimension()
{
    return dimension;
}

void ExpressionsStorage::get_maths_expressions_from_strings(std::vector<std::string> str_expr, std::vector<exprtk::expression<double>> &expr)
{

    table.add_constants();
    for (size_t i = 0; i < dimension; ++i)
    {
        table.add_variable("y" + std::to_string(i), result_at_point[i]);
    }

    for (size_t i = 0; i < dimension; ++i)
    {
        expr[i].register_symbol_table(table);
        parser.compile(str_expr[i], expr[i]);
    }
}
