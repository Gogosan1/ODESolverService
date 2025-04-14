package vkr.zelenev.ode_systems;

import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.Bean;
import jakarta.annotation.PostConstruct;

import java.util.*;

@Configuration

@ConfigurationProperties(prefix = "queues")
public class QueueConfig {
    private String responseQueueName;
    private List<String> queueNames;

    public String getResponseQueueName() {
        return responseQueueName;
    }

    public void setResponseQueueName(String responseQueueName) {
        this.responseQueueName = responseQueueName;
    }

    public List<String> getQueueNames() {
        return queueNames;
    }

    public void setQueueNames(List<String> queueNames) {
        this.queueNames = queueNames;
    }
}
