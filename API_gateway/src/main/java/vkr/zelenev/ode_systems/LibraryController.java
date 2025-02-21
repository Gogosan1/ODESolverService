package vkr.zelenev.ode_systems;

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

@RestController
@RequestMapping("/task")
public class LibraryController {

   private final QueueConfig queueConfig;

    private final RabbitTemplate rabbitTemplate;

    public LibraryController(RabbitTemplate rabbitTemplate, QueueConfig queueConfig) {
        this.rabbitTemplate = rabbitTemplate;
        this.queueConfig = queueConfig;
    }

    
    @PostMapping("/solve")
    public ResponseEntity<String> sendTask(@RequestBody Map<String, Object> jsonMessage)  throws Exception{

        //String taskID = UUID.randomUUID().toString();
        //jsonMessage.put("taskID", taskID);

        String method = (String) jsonMessage.get("solver");
        

        switch (method.toLowerCase()) {
            case "cpp": 
                rabbitTemplate.convertAndSend(queueConfig.getCppQueueName(), jsonMessage);
                break;
            case "julia":
                rabbitTemplate.convertAndSend(queueConfig.getJuliaQueueName(), jsonMessage);
                break;
            default:
                throw new Exception("This solver does not exitsts!");
        }


        return ResponseEntity.ok("Задача отправлена. ID: ");
    }
}
