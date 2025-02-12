#pragma once
#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv2.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_deriv.h>
#include <memory>
#include "../helpers/ExpressionsStorage.hpp"
#include "SolutionBuffer.hpp"
#include "Task.hpp"
#include "EnumMethods.hpp"
#include "../helpers/Publisher.hpp"
class Solver
{
private:
    static int func(double t, const double y[], double f[], void *params);
    static int jac(double t, const double y[], double *dfdy, double dfdt[], void *params);

public:
    void solve(std::shared_ptr<ExpressionsStorage>, Method method, std::shared_ptr<Task> taska, std::shared_ptr<Publisher> publisher);
};