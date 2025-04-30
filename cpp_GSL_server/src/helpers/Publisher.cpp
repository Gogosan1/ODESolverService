#include "Publisher.hpp"

Publisher::Publisher(AMQP::TcpChannel &channel, const std::string &queue, std::mutex *mutex, size_t batchSize)
    : channel(channel), queue(queue), batchSize(batchSize), publishMutex(mutex), debugMode(false)
{
}

void Publisher::sendResults(std::shared_ptr<SolutionBuffer> solution)
{
    std::lock_guard<std::mutex> lock(*publishMutex);
    // используется 1 раз, чтобы понять сколько результатов отправлять в одном чанке
    if (batchSize == 0)
    {
        nlohmann::json message;
        message["status"] = "processing";
        message["steps"] = nlohmann::json::array();
        message["sessionId"] = solution->getSessionId();
        for (const auto &step : solution->getResults())
        {
            message["steps"].push_back({{"t", step.first}, {"values", step.second}});
        }
        double size_of_chank_in_bytes = 16 * 1024;
        size_t json_size = static_cast<size_t>(std::floor(size_of_chank_in_bytes / static_cast<double>(message.dump().size())));
        if (json_size == 0)
            json_size = 1;
        // std::cout << json_size << std::endl;
        batchSize = json_size;
    }

    if (solution->getSize() >= batchSize || debugMode)
    {
        nlohmann::json message;
        message["status"] = "processing";
        message["steps"] = nlohmann::json::array();
        message["sessionId"] = solution->getSessionId();
        for (const auto &step : solution->getResults())
        {
            message["steps"].push_back({{"t", step.first}, {"values", step.second}});
        }
        solution->clearBuffer();

        channel.publish("", queue, message.dump());
    }
}

void Publisher::flush(std::shared_ptr<SolutionBuffer> solution)
{
    std::lock_guard<std::mutex> lock(*publishMutex);
    nlohmann::json message;
    message["status"] = "done";
    message["steps"] = nlohmann::json::array();
    message["sessionId"] = solution->getSessionId();
    for (const auto &step : solution->getResults())
    {
        message["steps"].push_back({{"t", step.first}, {"values", step.second}});
    }
    solution->clearBuffer();
    channel.publish("", queue, message.dump());
}