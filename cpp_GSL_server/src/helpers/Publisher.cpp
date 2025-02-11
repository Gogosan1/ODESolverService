#include "Publisher.hpp"

Publisher::Publisher(AMQP::TcpChannel &channel, const std::string &queue, size_t batchSize)
    : channel(channel), queue(queue), batchSize(batchSize), debugMode(false)
{
}

void Publisher::sendResults( std::shared_ptr<DoubleSolution> solution)
{
    if (solution->getSize() >= batchSize || debugMode)
    {
        nlohmann::json message;
        message["status"] = "processing";
        message["steps"] = nlohmann::json::array();

        for (const auto &step : solution->getResults())
        {
            message["steps"].push_back({{"t", step.first}, {"values", step.second}});
        }
        solution->clearBatch(batchSize);

        channel.publish("", queue, message.dump());
    }
}

void Publisher::flush( std::shared_ptr<DoubleSolution> solution)
{
    nlohmann::json message;
    message["status"] = "done";
    message["steps"] = nlohmann::json::array();

    for (const auto &step : solution->getResults())
    {
        message["steps"].push_back({{"t", step.first}, {"values", step.second}});
    }

    channel.publish("", queue, message.dump());
}