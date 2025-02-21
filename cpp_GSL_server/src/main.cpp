#include "consumer/Consumer.hpp"
#include <cstdlib>

int main()
{
    std::string rabbit_username = std::getenv("RABBITMQ_USERNAME");
    std::string rabbit_password = std::getenv("RABBITMQ_PASSWORD");
    std::string rabbit_host = std::getenv("RABBITMQ_HOST");
    std::string rabbit_port = std::getenv("RABBITMQ_PORT");
    const std::string connection_string = "amqp://" + std::string(rabbit_username) + ":" + std::string(rabbit_password) + "@" + std::string(rabbit_host) + ":" + std::string(rabbit_port);
    const std::string cpp_queue = std::getenv("CPP_QUEUE");
    const std::string response_queue = std::getenv("RESPONSE_QUEUE");
    Consumer consumer(cpp_queue, response_queue, connection_string);
    consumer.start(); // Запуск слушателя

    return 0;
}

// // TODO: взять файл из очереди
// nlohmann::json file = {
//     {"equations", {
//                       "y0",     // dy0/dt = y0
//                       "-100*y1" // dy1/dt = -100*y1
//                   }},
//     {"result_at_start_point", {1.0, 1.0}}, // Начальные условия y0 = 1, y1 = 0
//     {"h0", 0.01},                          // Шаг интегрирования
//     {"t_start", 0.0},                      // Начальное время
//     {"t_end", 0.1},                        // Конечное время
//     {"accuracy", 1e-8},                    // Точность
//     {"method", "msadams"}};                // Метод решения
// "matrix": [
//             ["a11", "a12", "a13"],
//             ["a21", "a22", "a23"],
//             ["a31", "a32", "a33"]
//         ]
