#include "Solver.hpp"

int Solver::func(double t, const double y[], double f[], void *params)
{
    ExpressionsStorage *storage = static_cast<ExpressionsStorage *>(params);

    for (int i = 0; i < storage->get_dimension(); i++)
    {
        f[i] = storage->expressions[i].value();
    }
    return GSL_SUCCESS;
}

int Solver::jac(double t, const double y[], double *dfdy, double dfdt[], void *params)
{
    ExpressionsStorage *storage = static_cast<ExpressionsStorage *>(params);

    gsl_matrix_view J = gsl_matrix_view_array(dfdy, storage->get_dimension(), storage->get_dimension());
    gsl_matrix *matrix = &J.matrix;

    for (int row = 0; row < storage->get_dimension(); row++)
    {
        for (int column = 0; column < storage->get_dimension(); column++)
        {
            gsl_matrix_set(matrix, row, column, storage->jacobi_matrix_expressions[row][column].value());
        }
    }
    return GSL_SUCCESS;
}

// добавить gsl_odeiv2_driver_set_nmax максимальное количество допустимых шагов

void Solver::solve(ExpressionsStorage *expr_storage, Method method, Task *task, Publisher *publisher)
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
    case Method::rkck:
        gsl_method = gsl_odeiv2_step_rkck;
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

    size_t dimension = expr_storage->get_dimension();

    gsl_odeiv2_system sys = {Solver::func, Solver::jac, dimension, expr_storage};

    gsl_odeiv2_driver *driver = gsl_odeiv2_driver_alloc_y_new(&sys, gsl_method, task->h0, task->accuracy, task->accuracy);

    // Установим минимальный и максимальный шаг и  максимальное количество шагов
    gsl_odeiv2_driver_set_hmin(driver, task->accuracy * 1e-2);
    gsl_odeiv2_driver_set_hmax(driver, (task->t_end - task->t_start) / 100.0);
    gsl_odeiv2_driver_set_nmax(driver, 1e6);

    std::shared_ptr<SolutionBuffer> solution = std::make_shared<SolutionBuffer>(dimension, task->sessionId);

    solution->add_result_at_point(0, task->start_conditions);

    double ti = task->t_start;
    while (ti <= task->t_end)
    {

        int status = gsl_odeiv2_driver_apply(driver, &ti, ti + task->h0, expr_storage->result_at_point.data());

        if (status != GSL_SUCCESS)
        {
            throw std::runtime_error("Integration error at t = " + std::to_string(ti) +
                                     ": " + gsl_strerror(status));
        }

        solution->add_result_at_point(ti, expr_storage->result_at_point);
        publisher->sendResults(solution);
    }

    gsl_odeiv2_driver_free(driver);

    // Финальная отправка
    publisher->flush(solution);
}
