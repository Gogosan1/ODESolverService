package vkr.zelenev.ode_systems;

import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.context.annotation.Configuration;
import lombok.Getter;
import lombok.Setter;


@Configuration
@ConfigurationProperties(prefix = "queues")
@Getter
@Setter
public class QueueConfig {
    private String cppQueueName;
    private String juliaQueueName;
    private String responseQueueName;
}
