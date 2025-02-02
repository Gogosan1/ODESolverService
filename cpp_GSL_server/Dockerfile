# Используем официальный образ Ubuntu
FROM ubuntu:20.04

# Устанавливаем временную зону на Новосибирск без запроса
RUN DEBIAN_FRONTEND=noninteractive apt-get update && apt-get install -y \
    tzdata \
    && ln -fs /usr/share/zoneinfo/Asia/Novosibirsk /etc/localtime \
    && dpkg-reconfigure --frontend noninteractive tzdata \
    && rm -rf /var/lib/apt/lists/*

# Устанавливаем остальные зависимости
RUN DEBIAN_FRONTEND=noninteractive apt-get update && apt-get install -y \
    libssl-dev \
    build-essential \
    cmake \
    git \
    libboost-all-dev \
    libgsl-dev \
    libev-dev \
    wget \
    g++ \
    nlohmann-json3-dev \
    && rm -rf /var/lib/apt/lists/*

# Устанавливаем AMQP-CPP
RUN git clone --recurse-submodules https://github.com/CopernicaMarketingSoftware/AMQP-CPP.git /AMQP-CPP \
    && cd /AMQP-CPP && cmake -DBUILD_SHARED_LIBS=ON -DAMQP-CPP_LINUX_TCP=ON . && make && make install

# Копируем исходный код проекта в контейнер
COPY . /app

# Устанавливаем рабочую директорию
WORKDIR /app

# Создаем и собираем проект с помощью CMake
RUN mkdir build && cd build && cmake .. && cmake --build .

# Указываем команду для запуска приложения
CMD ["./build/consumer"]
