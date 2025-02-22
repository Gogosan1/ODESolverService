![Spring](https://img.shields.io/badge/spring-%236DB33F.svg?style=for-the-badge&logo=spring&logoColor=white)
![Java](https://img.shields.io/badge/java-%23ED8B00.svg?style=for-the-badge&logo=openjdk&logoColor=white)
![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Julia](https://img.shields.io/badge/-Julia-9558B2?style=for-the-badge&logo=julia&logoColor=white)
![RabbitMQ](https://img.shields.io/badge/Rabbitmq-FF6600?style=for-the-badge&logo=rabbitmq&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white)
![Docker](https://img.shields.io/badge/docker-%230db7ed.svg?style=for-the-badge&logo=docker&logoColor=white)
![Grafana](https://img.shields.io/badge/grafana-%23F46800.svg?style=for-the-badge&logo=grafana&logoColor=white)

# Выпускная квалификационная работа бакалавра

_Тема: Разработка программного обеспечения для расчета поведения динамических и гибридных систем._

## Описание

**Цель работы** — разработка и реализация алгоритма для получения универсального доступа к различным библиотекам численных решателей.

Результатом работы будет являтся программный модуль, к которому возможно подключать любые разработанные библиотеки численных решателей.

![Схема системы ПО](https://github.com/user-attachments/assets/ef51008a-f6eb-40ef-af63-251c53e1cb1e)

> [!NOTE]
>
> - **Цветом выделена та часть системы, которая реализована мной**
>   - Зеленым на схеме отображен сам модуль
>   - Желтым отображены подключенные численные решатели

При разработке программного обеспечения было принято решение делать его через **микросервисную архитектуру**.
В качестве брокера сообщений используется rabbitMQ сконфигурированный на api шлюзе.

**Структура проекта:**

```
ODESolverService/
├── API_gateway/ #Содержит код шлюза API, написанный на Java с использованием Spring Boot.
│ ├── src/
│ │ ├── main/ #Основной код шлюза
│ │ └── test/ #Тесты для шлюза
│ ├── pom.xml #Maven файл для конфигурации
| |── Dockerfile #Докер образ для шлюза
|
├── Julia_server/
│ ├── server.jl #Основной код решателя ОДУ на julia
│ └── Dockerfile #Докер образ для julia server
|
├── cpp_GSL_server/
│ ├── src/ #Исходный код сервера решателя на C++
│ ├── CMakeLists.txt #Файл конфигурации для сборки проекта с помощью CMake.
│ └── Dockerfile #Докер образ для C++ server
|
├── .env.example #Пример файла окружения с переменными среды.
├── README.md
├── compose.yaml #Файл конфигурации для Docker Compose, описывающий, как запускать многоконтейнерное приложение.
```
**spring boot server (API GATEWAY)**

- [ ] добавить описание api
- [ ] добавить описание json шаблонов для отправки по api
- [ ] добавить мониторинг взаимодействия микросервисов
- [x] ~~создать docker image~~
- [x] ~~Внедрить .env файл для общей конфигурации подключений и названия очередей~~

**CPP server (GSL)**

- [x] ~~написать функцию решателя~~
- [x] ~~реализовать выбор возможных методов решения~~
- [x] ~~реализовать парсер, для извлечения уравнений из строкового формата~~
- [x] ~~реализовать асинхронную обработку задач~~
- [ ] реализовать отправку решения частями
- [ ] реализовать механизм отладки (возвращения на предыдущий шаг)
- [x] ~~создать docker image~~
- [x] ~~написать CMake~~

**julia server (DifferentialEquations)**

- [x] ~~написать функцию решателя~~
- [x] ~~реализовать выбор возможных методов решения~~
- [x] ~~реализовать парсер, для извлечения уравнений из строкового формата~~
- [ ] реализовать асинхронную обработку задач
- [ ] реализовать отправку решения частями
- [x] ~~создать docker image~~

**Selfwrite solver server**

- [ ] интегрировать в систему код сервера Алексея
- [ ] возможно добавить в код логику парсера из CPP server

### Тесты

- [ ] весь код обернуть тестами
- [ ] протестить работу всех методов
- [ ] сравнить скорость обработки
