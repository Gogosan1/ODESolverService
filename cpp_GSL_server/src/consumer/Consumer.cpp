#include "Consumer.hpp"

// Конструктор
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

        std::string rawMessage(message.body(), message.bodySize());                                  // Получаем строку с учетом размера
        rawMessage.erase(std::remove(rawMessage.begin(), rawMessage.end(), '\n'), rawMessage.end()); // Удаляем \n
        rawMessage.erase(std::remove(rawMessage.begin(), rawMessage.end(), '\r'), rawMessage.end()); // Удаляем \r

        nlohmann::json file = nlohmann::json::parse(rawMessage); // Парсим JSON

        std::unique_ptr<JSONHelper> helper = std::make_unique<JSONHelper>();
        helper->upload_from_json(file, task, &method);

        std::unique_ptr<ExpressionsParser> converter = std::make_unique<ExpressionsParser>();
        converter->convert_string_to_maths(task);

        Solver solver;
        std::unique_ptr<Solution> solution = solver.solve(task, method);

        std::string json_string = solution->get_data();

        nlohmann::json responseJson = {
            {"status", "success"},
            {"message", json_string}};

        std::string responseStr = responseJson.dump();
        // Отправляем ответ в responseQueue
        channel.publish("", responseQueue, responseStr);

        // Подтверждаем сообщение
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
    std::cout << "nj\n";
    channel.consume(queueName).onReceived([this](const AMQP::Message &message, uint64_t deliveryTag, bool redelivered)
                                          { this->onMessageReceived(message, deliveryTag, redelivered); });

    ev_run(loop, 0); // Запускаем event loop
    std::cout << "consumer start";
}
