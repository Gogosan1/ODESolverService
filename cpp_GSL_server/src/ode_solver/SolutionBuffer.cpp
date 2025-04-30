#include "SolutionBuffer.hpp"

void SolutionBuffer::add_result_at_point(double point, std::vector<double> results)
{
    this->results.emplace_back(point, results);
}

SolutionBuffer::SolutionBuffer(int dimension, std::string sessionId)
{
    this->sessionId = sessionId;
    this->dimension = dimension;
}

const std::vector<std::pair<double, std::vector<double>>> &SolutionBuffer::getResults() const
{
    return results;
}

void SolutionBuffer::clearBatch(size_t batchSize)
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

size_t SolutionBuffer::getSize() const
{
    return results.size();
}

void SolutionBuffer::clearBuffer()
{
    results.clear();
}

std::string SolutionBuffer::getSessionId()
{
    return sessionId;
}
