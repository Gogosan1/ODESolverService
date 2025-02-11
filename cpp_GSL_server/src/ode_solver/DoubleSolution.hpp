#pragma once
#include <string>
#include <vector>

class DoubleSolution
{
private:
    std::vector<std::pair<double, std::vector<double>>> results;
    int dimension;

public:
    void add_result_at_point(double point, std::vector<double> results);
    DoubleSolution(int dimension);

    const std::vector<std::pair<double, std::vector<double>>> &getResults() const;
    void clearBatch(size_t batchSize);
    size_t getSize() const;
};
