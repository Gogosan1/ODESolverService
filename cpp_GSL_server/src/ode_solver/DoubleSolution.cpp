#include "DoubleSolution.hpp"

void DoubleSolution::add_result_at_point(double point, std::vector<double> results)
{
    this->results.emplace_back(point, results);
}

DoubleSolution::DoubleSolution(int dimension)
{
    this->dimension = dimension;
}

const std::vector<std::pair<double, std::vector<double>>> &DoubleSolution::getResults() const
{
    return results;
}

void DoubleSolution::clearBatch(size_t batchSize)
{
    if (results.size() >= batchSize)
    {
        results.erase(results.begin(), results.begin() + batchSize);
    }
    else
    {
        results.clear();
    }
}

size_t DoubleSolution::getSize() const {
    return results.size();
}