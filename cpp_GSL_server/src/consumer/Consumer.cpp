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

        //         std::string rawMessage = R"({
        //   "equations": ["y0", "-100*y1" ],
        //   "initial_conditions": [1.0, 1.0],
        //   "h0": 0.01,
        //   "t_start": 0.0,
        //   "t_end": 0.1,
        //   "accuracy": 1e-8,
        //   "method": "bsimp",
        //   "jacobi_matrix": [
        //     ["1.0", "0"],
        //     ["0", "-100"]
        //   ]
        // })";
        std::string rawMessage(message.body(), message.bodySize());
        rawMessage.erase(std::remove(rawMessage.begin(), rawMessage.end(), '\n'), rawMessage.end()); // Удаляем \n
        rawMessage.erase(std::remove(rawMessage.begin(), rawMessage.end(), '\r'), rawMessage.end()); // Удаляем \r
        nlohmann::json file = nlohmann::json::parse(rawMessage);
        JSONHelper helper;
        helper.upload_from_json(file, task, &method);

        std::shared_ptr<ExpressionsStorage> expr_storage = std::make_shared<ExpressionsStorage>(task);

        Solver solver;
        std::unique_ptr<Solution> solution = solver.solve(expr_storage, method, task);

        std::string json_string = solution->get_data();
        std::cout << json_string;
        nlohmann::json responseJson = {
            {"status", "success"},
            {"message", json_string}};
        std::string responseStr = responseJson.dump();

        //  Отправляем ответ в responseQueue
        channel.publish("", responseQueue, responseStr);

        // // Подтверждаем сообщение
        channel.ack(deliveryTag);

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
