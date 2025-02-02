#include "Solver.hpp"
#include <iostream>
// TODO: вынести задание функции в отдельный класс, для возможности асинхронного решения
// static временное решение
int Solver::compute_ODEs_right_sides(double t, const double y[], double f[], void *params)
{
    Task *task = static_cast<Task *>(params);

    int dimension = task->equations_strings.size();

    for (int i = 0; i < dimension; i++)
    {
        f[i] = task->expressions[i].value();
    }

    return GSL_SUCCESS;
}

int Solver::jac(double t, const double y[], double *dfdy, double dfdt[], void *params)
{
    Task *task = static_cast<Task *>(params);
    int dimension = task->equations_strings.size();
    gsl_matrix_view J = gsl_matrix_view_array(dfdy, dimension, dimension);
    gsl_matrix *matrix = &J.matrix;

    // double h = 1e-6; // Шаг для численного дифференцирования
    // gsl_function F;
    // F.function = nullptr;

    // for (int row = 0; row < dimension; row++)
    // {
    //     exprtk::expression<double> expr = task->expressions[row];

    //     for (int column = 0; column < dimension; column++)
    //     {
    //         // Параметры для дифференцирования

    //         // Численное дифференцирование по переменной y[column]
    //         double result, abserr;
    //         int status = gsl_deriv_central(&F, y[column], h, &result, &abserr);
    //         std::cout << "Rres " << result << " abser " << abserr << "\n";
    //         gsl_matrix_set(matrix, row, column, result);
    //     }
    // }

    // for (int row = 0; row < dimension; row++)
    // {
    //     for (int column = 0; column < dimension; column++)
    //     {

    //         gsl_matrix_set(matrix, row, column, task->)
    //     }
    // }
    return GSL_SUCCESS;
}

// TODO: определиться лучше постепенно присылать решение или сразу целиком?
// добавить gsl_odeiv2_driver_set_nmax максимальное количество допустимых шагов
// добавить задание jacobian
std::unique_ptr<Solution> Solver::solve(std::shared_ptr<Task> task, Method method)
{
    const gsl_odeiv2_step_type *gsl_method;
    switch (method)
    {
    case Method::rk2:
        gsl_method = gsl_odeiv2_step_rk2;
        break;
    case Method::rk4:
        gsl_method = gsl_odeiv2_step_rk4;
        break;
    case Method::rkf45:
        gsl_method = gsl_odeiv2_step_rkf45;
        break;
    case Method::rk8pd:
        gsl_method = gsl_odeiv2_step_rk8pd;
        break;
    case Method::rk1imp:
        gsl_method = gsl_odeiv2_step_rk1imp;
        break;
    case Method::rk2imp:
        gsl_method = gsl_odeiv2_step_rk2imp;
        break;
    case Method::rk4imp:
        gsl_method = gsl_odeiv2_step_rk4imp;
        break;
    case Method::bsimp:
        gsl_method = gsl_odeiv2_step_bsimp;
        break;
    case Method::msadams:
        gsl_method = gsl_odeiv2_step_msadams;
        break;
    case Method::msbdf:
        gsl_method = gsl_odeiv2_step_msbdf;
        break;
    default:
        throw "unknown method of solver";
        break;
    }

    size_t dimension = task->equations_strings.size();

    gsl_odeiv2_system sys = {Solver::compute_ODEs_right_sides, nullptr /*Solver::jac*/, dimension, task.get()}; // вместо nullptr передавать jacobian

    gsl_odeiv2_driver *driver = gsl_odeiv2_driver_alloc_y_new(&sys, gsl_method, task->h0, task->accuracy, task->accuracy);

    // Установим минимальный и максимальный шаг
    // gsl_odeiv2_driver_set_hmin(driver, task->accuracy);
    // gsl_odeiv2_driver_set_hmax(driver, 1.0);
    std::unique_ptr<Solution> solution = std::make_unique<Solution>(dimension);

    double ti = task->t_start;
    while (ti <= task->t_end)
    {
        int status_rk4 = gsl_odeiv2_driver_apply(driver, &ti, ti + task->h0, task->result_at_point.data());

        solution->add_result_at_point(ti, task->result_at_point);
        // if (status_rk4 != GSL_SUCCESS)
        // {
        //     fprintf(stderr, "Ошибка при интегрировании: %s\n", gsl_strerror(status_rk4));
        //     break;
        // }
    }

    gsl_odeiv2_driver_free(driver);
    return std::move(solution);
}
