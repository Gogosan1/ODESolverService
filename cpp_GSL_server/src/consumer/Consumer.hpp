#pragma once
#include <amqpcpp.h>
#include <amqpcpp/libev.h>
#include <ev.h>
#include <string>
#include <iostream>
#include <future>
#include <nlohmann/json.hpp>
#include "../helpers/JSONHelper.hpp"
#include "../helpers/ExpressionsStorage.hpp"
#include "../ode_solver/Task.hpp"
#include "../ode_solver/EnumMethods.hpp"
#include "../ode_solver/Solver.hpp"
#include "../helpers/Publisher.hpp"

class Consumer
{
public:
    Consumer(const std::string &queueName, const std::string &responseQueue);
    void start();

private:
    std::vector<std::future<void>> futures;
    struct ev_loop *loop;
    AMQP::LibEvHandler handler;
    AMQP::TcpConnection connection;
    AMQP::TcpChannel channel;
    std::string queueName;
    std::string responseQueue;

    void onMessageReceived(const AMQP::Message &message, uint64_t deliveryTag, bool redelivered);
};
