![Spring](https://img.shields.io/badge/spring-%236DB33F.svg?style=for-the-badge&logo=spring&logoColor=white)
![Java](https://img.shields.io/badge/java-%23ED8B00.svg?style=for-the-badge&logo=openjdk&logoColor=white)
![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Julia](https://img.shields.io/badge/-Julia-9558B2?style=for-the-badge&logo=julia&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white)
![Docker](https://img.shields.io/badge/docker-%230db7ed.svg?style=for-the-badge&logo=docker&logoColor=white)
![Grafana](https://img.shields.io/badge/grafana-%23F46800.svg?style=for-the-badge&logo=grafana&logoColor=white)
# Выпускная квалификационная работа бакалавра

*Тема: Разработка программного обеспечения для расчета поведения динамических и гибридных систем.*

## Описание

**Цель работы** — разработка и реализация алгоритма для получения универсального доступа к различным библиотекам численных решателей.

Результатом работы будет являтся программный модуль, к которому возможно подключать любые разработанные библиотеки численных решателей.

![Схема системы ПО](https://github.com/user-attachments/assets/ef51008a-f6eb-40ef-af63-251c53e1cb1e)

> [!NOTE]
> - **Цветом выделена та часть системы, которая реализована мной** 
>   - {green}(Зеленым на схеме отображен сам модуль)
>   - {yellow}(Желтым отображены подключенные численные решатели)

**spring boot server (API GATEWAY)**

- [ ] добавить описание api
- [ ] добавить описание json шаблонов для отправки по api
- [ ] добавить мониторинг взаимодействия микросервисов
- [X] ~~создать docker image~~
- [X] ~~Внедрить .env файл для общей конфигурации подключений и названия очередей~~

**CPP server (GSL)**

- [X] ~~написать функцию решателя~~
- [X] ~~реализовать выбор возможных методов решения~~
- [X] ~~реализовать парсер, для извлечения уравнений из строкового формата~~ 
- [X] ~~реализовать асинхронную обработку задач~~
- [ ] реализовать отправку решения частями
- [ ] реализовать механизм отладки (возвращения на предыдущий шаг)
- [X] ~~создать docker image~~
- [X] ~~написать CMake~~

**julia server (DifferentialEquations)**

- [X] ~~написать функцию решателя~~
- [X] ~~реализовать выбор возможных методов решения~~
- [X] ~~реализовать парсер, для извлечения уравнений из строкового формата~~ 
- [ ] реализовать асинхронную обработку задач
- [ ] реализовать отправку решения частями
- [X] ~~создать docker image~~

**Selfwrite solver server**

- [ ] интегрировать в систему код сервера Алексея
- [ ] возможно добавить в код логику парсера из CPP server

### Тесты

- [ ] весь код обернуть тестами
- [ ] протестить работу всех методов
- [ ] сравнить скорость обработки

