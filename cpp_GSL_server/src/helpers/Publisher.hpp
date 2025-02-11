#pragma once
#include <string>
#include <amqpcpp.h>
#include <amqpcpp/libev.h>
#include "../ode_solver/DoubleSolution.hpp"
#include <nlohmann/json.hpp>
class Publisher
{
private:
    AMQP::TcpChannel &channel;
    std::string queue;
    size_t batchSize;
    bool debugMode;

public:
    Publisher(AMQP::TcpChannel &channel, const std::string &queue, size_t batchSize);

    void sendResults( std::shared_ptr<DoubleSolution> solution);
    void flush( std::shared_ptr<DoubleSolution> solution);

   // void setDebugMode(bool mode);
   // void setBatchSize(size_t size);
};