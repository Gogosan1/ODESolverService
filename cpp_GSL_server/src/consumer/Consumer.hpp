#pragma once
#include <amqpcpp.h>
#include <amqpcpp/libev.h>
#include <ev.h>
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include "../helpers/JSONHelper.hpp"
#include "../helpers/ExpressionsParser.hpp"
#include "../ode_solver/Task.hpp"
#include "../ode_solver/EnumMethods.hpp"
#include "../ode_solver/Solution.hpp"
#include "../ode_solver/Solver.hpp"

class Consumer
{
public:
    Consumer(const std::string &queueName, const std::string &responseQueue);
    void start();

private:
    struct ev_loop *loop;
    AMQP::LibEvHandler handler;
    AMQP::TcpConnection connection;
    AMQP::TcpChannel channel;
    std::string queueName;
    std::string responseQueue;

    void onMessageReceived(const AMQP::Message &message, uint64_t deliveryTag, bool redelivered);
};
