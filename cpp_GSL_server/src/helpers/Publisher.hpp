#pragma once
#include <string>
#include <amqpcpp.h>
#include <amqpcpp/libev.h>
#include "../ode_solver/SolutionBuffer.hpp"
#include <nlohmann/json.hpp>
#include <mutex>

class Publisher
{
private:
    AMQP::TcpChannel &channel;
    std::string queue;
    size_t batchSize;
    bool debugMode;
    std::mutex *publishMutex; // ✅ Мьютекс для защиты `publish()`
public:
    Publisher(AMQP::TcpChannel &channel, const std::string &queue, std::mutex *publishMutex, size_t batchSize = 0);

    void sendResults(std::shared_ptr<SolutionBuffer> solution);
    void flush(std::shared_ptr<SolutionBuffer> solution);

    // void setDebugMode(bool mode);
    // void setBatchSize(size_t size);
};