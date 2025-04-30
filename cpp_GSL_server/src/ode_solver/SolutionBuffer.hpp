#pragma once
#include <string>
#include <vector>

class SolutionBuffer
{
private:
    std::vector<std::pair<double, std::vector<double>>> results;
    int dimension;
    std::string sessionId;

public:
    void add_result_at_point(double point, std::vector<double> results);
    SolutionBuffer(int dimension, std::string sessionId);

    const std::vector<std::pair<double, std::vector<double>>> &getResults() const;
    void clearBatch(size_t batchSize);
    size_t getSize() const;
    void clearBuffer();
    std::string getSessionId();
};
