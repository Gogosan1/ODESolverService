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

// TODO: определиться лучше постепенно присылать решение или сразу целиком?
// добавить gsl_odeiv2_driver_set_nmax максимальное количество допустимых шагов

void Solver::solve(std::shared_ptr<ExpressionsStorage> expr_storage, Method method, std::shared_ptr<Task> task, std::shared_ptr<Publisher> publisher)
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

    gsl_odeiv2_system sys = {Solver::func, Solver::jac, dimension, expr_storage.get()};

    gsl_odeiv2_driver *driver = gsl_odeiv2_driver_alloc_y_new(&sys, gsl_method, task->h0, task->accuracy, task->accuracy);

    // Установим минимальный и максимальный шаг
    // gsl_odeiv2_driver_set_hmin(driver, task->accuracy);
    // gsl_odeiv2_driver_set_hmax(driver, 1.0);
    std::shared_ptr<DoubleSolution> solution = std::make_shared<DoubleSolution>(dimension);

    solution->add_result_at_point(0, task->start_conditions);

    double ti = task->t_start;
    while (ti <= task->t_end)
    {

        int status_rk4 = gsl_odeiv2_driver_apply(driver, &ti, ti + task->h0, expr_storage->result_at_point.data());

        // if (status_rk4 != GSL_SUCCESS)
        // {
        //     fprintf(stderr, "Ошибка при интегрировании: %s\n", gsl_strerror(status_rk4));
        //     break;
        // }
        solution->add_result_at_point(ti, expr_storage->result_at_point);
        publisher->sendResults(solution);
    }

    gsl_odeiv2_driver_free(driver);

    // Финальная отправка
    publisher->flush(solution);
}
