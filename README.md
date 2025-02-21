# Выпускная квалификационная работа бакалавра

*Тема: Разработка программного обеспечения для расчета поведения динамических и гибридных систем.*

## Описание

**Цель работы** — разработка и реализация алгоритма для получения универсального доступа к различным библиотекам численных решателей.

Результатом работы будет являтся программный модуль, к которому возможно подключать любые разработанные библиотеки численных решателей.

![Схема системы](/Images/система.png)

> [!NOTE]
> <span style="color: green; font-weight: bold;">Зеленым на схеме отображен сам модуль</span>
>
> [!NOTE]
> <span style="color: yellow; font-weight: bold;">Желтым отображены подключенные численные решатели</span>

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

