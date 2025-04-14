package vkr.zelenev.ode_systems;

import org.springframework.amqp.core.Queue;
import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.amqp.support.converter.Jackson2JsonMessageConverter;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import vkr.zelenev.ode_systems.QueueConfig;

import org.springframework.amqp.rabbit.connection.ConnectionFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.context.properties.EnableConfigurationProperties;
import org.springframework.amqp.core.AmqpAdmin;

import java.util.*;
import jakarta.annotation.PostConstruct;

@Configuration
@EnableConfigurationProperties(QueueConfig.class)
public class RabbitConfig {

    @Autowired
    private AmqpAdmin amqpAdmin;

    @Autowired
    private QueueConfig queueConfig;

    @Bean
    public RabbitTemplate rabbitTemplate(ConnectionFactory connectionFactory) {
        RabbitTemplate template = new RabbitTemplate(connectionFactory);
        template.setMessageConverter(jackson2JsonMessageConverter());
        return template;
    }

    @Bean
    public Jackson2JsonMessageConverter jackson2JsonMessageConverter() {
        return new Jackson2JsonMessageConverter();
    }

    @PostConstruct
    public void createQueues() {
        List<String> queueNames = queueConfig.getQueueNames();

        for (String queueName : queueNames) {
            amqpAdmin.declareQueue(new Queue(queueName, true));
        }
    }
}
