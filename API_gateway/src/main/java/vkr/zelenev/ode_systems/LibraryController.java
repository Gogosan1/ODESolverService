package vkr.zelenev.ode_systems;

import java.util.HashSet;
import java.util.Map;
import java.util.UUID;
import java.util.stream.Stream;

import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.web.bind.annotation.*;
import org.springframework.http.ResponseEntity;
import com.fasterxml.jackson.databind.ObjectMapper;
import vkr.zelenev.ode_systems.QueueConfig;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.amqp.core.Message;

import io.micrometer.core.instrument.MeterRegistry;
import io.micrometer.core.instrument.Counter;


@RestController
@RequestMapping("/task")
public class LibraryController {

    private final QueueConfig queueConfig;

    private final RabbitTemplate rabbitTemplate;
   // private final Counter restRequestCounter;

    public LibraryController(RabbitTemplate rabbitTemplate/*, MeterRegistry registry*/, QueueConfig queueConfig) {
        this.rabbitTemplate = rabbitTemplate;
        this.queueConfig = queueConfig;
      //  this.restRequestCounter = registry.counter("http_requests_post_total");
        // this.restRequestCounter = registry.counter("api_requests", "endpoint", "/task/solve");
    }

    
    @PostMapping("/solve")
    public ResponseEntity<String> sendTask(@RequestBody Map<String, Object> jsonMessage)  throws Exception{

        String solverType = (String) jsonMessage.get("solver");        
    
        if (queueConfig.getQueueNames().contains(solverType.toLowerCase()))
        {
            rabbitTemplate.convertAndSend(solverType.toLowerCase(), jsonMessage);
        }
        else
        {
            throw new Exception("This solver does not exitsts!");
        }

      //  restRequestCounter.increment();
        return ResponseEntity.ok("Задача отправлена. ID: ");
    }
}
