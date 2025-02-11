#include "Consumer.hpp"

Consumer::Consumer(const std::string &queue, const std::string &responseQueue)
    : loop(EV_DEFAULT), handler(loop), connection(&handler, AMQP::Address("amqp://guest:guest@rabbitmq:5672/")),
      channel(&connection), queueName(queue), responseQueue(responseQueue)
{

    // Объявляем основную очередь
    channel.declareQueue(queueName, AMQP::durable);

    // Объявляем очередь для ответов
    channel.declareQueue(responseQueue, AMQP::durable);
}

// Метод обработки сообщений
void Consumer::onMessageReceived(const AMQP::Message &message, uint64_t deliveryTag, bool redelivered)
{
    try
    {

        std::shared_ptr<Task> task = std::make_shared<Task>();
        Method method;

        std::string rawMessage(message.body(), message.bodySize());
        rawMessage.erase(std::remove(rawMessage.begin(), rawMessage.end(), '\n'), rawMessage.end()); // Удаляем \n
        rawMessage.erase(std::remove(rawMessage.begin(), rawMessage.end(), '\r'), rawMessage.end()); // Удаляем \r
        nlohmann::json file = nlohmann::json::parse(rawMessage);
        JSONHelper helper;
        helper.upload_from_json(file, task, &method);

        std::shared_ptr<ExpressionsStorage> expr_storage = std::make_shared<ExpressionsStorage>(task);

        Solver solver;
        std::shared_ptr<Publisher> publihser = std::make_shared<Publisher>(channel, responseQueue, 10);
        solver.solve(expr_storage, method, task, publihser);

        std::cout << "Response sent to " << responseQueue << ": " << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error processing message: " << e.what() << std::endl;
    }
}

// Запуск слушателя
void Consumer::start()
{
    channel.consume(queueName).onReceived([this](const AMQP::Message &message, uint64_t deliveryTag, bool redelivered)
                                          { this->onMessageReceived(message, deliveryTag, redelivered); });

    ev_run(loop, 0); // Запускаем event loop
}
