cmake_minimum_required(VERSION 3.10)
project(ConsumerProject)

set(CMAKE_CXX_STANDARD 17)

# TODO: Можно отдельно создать bash файл для установки необходимых библиотек amqpcpp, gsl, nlohman/json 

# Указываем пути к заголовочным файлам
include_directories(${CMAKE_SOURCE_DIR}/src/consumer)
include_directories(${CMAKE_SOURCE_DIR}/src/helpers)
include_directories(${CMAKE_SOURCE_DIR}/src/ode_solver)
include_directories(${CMAKE_SOURCE_DIR}/exprtk)

# Укажите путь к заголовкам AMQP-CPP, если необходимо
include_directories(/usr/local/include)  # или путь, где установлена библиотека
include_directories(${GSL_INCLUDE_DIRS})


find_package(GSL REQUIRED)
find_package(nlohmann_json)
# Добавляем исходные файлы в проект
set(SOURCES src/main.cpp src/consumer/Consumer.cpp src/helpers/JSONHelper.cpp src/helpers/ExpressionsStorage.cpp src/ode_solver/DoubleSolution.cpp src/ode_solver/Solver.cpp src/helpers/Publisher.cpp)
#
# Создаем исполняемый файл
add_executable(consumer ${SOURCES})

# Указываем библиотеки для линковки
target_link_libraries(consumer amqpcpp pthread ${GSL_LIBRARIES} gsl gslcblas m nlohmann_json::nlohmann_json dl ev)
#
#TODO: добавить отдельно gsl библиотеку для линковки

