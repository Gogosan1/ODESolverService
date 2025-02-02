#include "consumer/Consumer.hpp"

int main()
{
    Consumer consumer("cppQueue", "responseQueue");
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
// // rk2,
// // rk4, works ok
// // rkf45,
// // rkck,
// // rk8pd,
// // rk1imp, требует якобиана
// // rk2imp, требует якобиана
// // rk4imp, требует якобиана
// // bsimp, требует якобиана
// // msadams,
// // msbdf, требует якобиана
