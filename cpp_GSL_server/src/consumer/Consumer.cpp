#include "Consumer.hpp"

Consumer::Consumer(const std::string &queue, const std::string &responseQueue, const std::string &address)
    : loop(EV_DEFAULT), handler(loop), connection(&handler, AMQP::Address(address /*"amqp://guest:guest@rabbitmq:5672/"*/)),
      channel(&connection), queueName(queue), responseQueue(responseQueue)
{

    // Объявляем основную очередь
    channel.declareQueue(queueName, AMQP::durable);

    // Объявляем очередь для ответов
    channel.declareQueue(responseQueue, AMQP::durable);

    std::cout << "✅ CPP сервер запущен и слушает cppQueue...\n";
}

// Метод обработки сообщений
void Consumer::onMessageReceived(const AMQP::Message &message, uint64_t deliveryTag, bool redelivered)
{

    futures.push_back(std::async(std::launch::async, [this, &message, deliveryTag]()
                                 {
        try
        {
            Task task;
            Method method;
            std::string rawMessage(message.body(), message.bodySize());
            rawMessage.erase(std::remove(rawMessage.begin(), rawMessage.end(), '\n'), rawMessage.end()); // Удаляем \n
            rawMessage.erase(std::remove(rawMessage.begin(), rawMessage.end(), '\r'), rawMessage.end()); // Удаляем \r
            nlohmann::json file;
            try {
                file = nlohmann::json::parse(rawMessage);
            }
            catch (const nlohmann::json::parse_error& e) {
                std::cerr << "JSON parsing error: " << e.what() << std::endl;
                return; // Выход из функции, если произошла ошибка при парсинге
            }

            JSONHelper helper;
            helper.upload_from_json(file, &task, &method);
            
            std::cout << "Loaded task and method from JSON." << std::endl;

            ExpressionsStorage expr_storage(&task);

            Publisher publihser(channel, responseQueue, &this->publishMutex);
            
            Solver solver;
            solver.solve(&expr_storage, method, &task, &publihser);

            std::cout << "Solver finished solving." << std::endl;

            {
                std::lock_guard<std::mutex> lock(this->ackMutex);
                channel.ack(deliveryTag);
            }

            std::cout << "Response sent to: " << responseQueue << ". Task id: " << task.sessionId <<  std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error processing message: " << e.what() << std::endl;
        } }));
}

// Запуск слушателя
void Consumer::start()
{
    channel.consume(queueName).onReceived([this](const AMQP::Message &message, uint64_t deliveryTag, bool redelivered)
                                          { this->onMessageReceived(message, deliveryTag, redelivered); });

    ev_run(loop, 0); // Запускаем event loop
}
