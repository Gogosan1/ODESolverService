#pragma once
#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv2.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_deriv.h>
#include <memory>
#include "../helpers/ExpressionsStorage.hpp"
#include "Solution.hpp"
#include "Task.hpp"
#include "EnumMethods.hpp"
class Solver
{
private:
    static int func(double t, const double y[], double f[], void *params);
    static int jac(double t, const double y[], double *dfdy, double dfdt[], void *params);

public:
    std::unique_ptr<Solution> solve(std::shared_ptr<ExpressionsStorage>, Method method, std::shared_ptr<Task> taska);
};