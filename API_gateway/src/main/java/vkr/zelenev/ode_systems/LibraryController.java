package vkr.zelenev.ode_systems;

import java.util.Map;
import java.util.UUID;
import java.util.stream.Stream;

import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.web.bind.annotation.*;
import org.springframework.http.ResponseEntity;
import com.fasterxml.jackson.databind.ObjectMapper;

import org.springframework.amqp.core.Message;

@RestController
@RequestMapping("/task")
public class LibraryController {

    private final RabbitTemplate rabbitTemplate;

    public LibraryController(RabbitTemplate rabbitTemplate) {
        this.rabbitTemplate = rabbitTemplate;
    }

    @PostMapping("/solve")
    public ResponseEntity<String> sendTask(@RequestBody Map<String, Object> jsonMessage)  throws Exception{

        //String taskID = UUID.randomUUID().toString();
        //jsonMessage.put("taskID", taskID);

        String method = (String) jsonMessage.get("solver");
        

        switch (method.toLowerCase()) {
            case "cpp": 
                rabbitTemplate.convertAndSend("cppQueue", jsonMessage);
                break;
            case "julia":
                rabbitTemplate.convertAndSend("juliaQueue", jsonMessage);
                break;
            default:
                throw new Exception("This solver does not exitsts!");
        }


        return ResponseEntity.ok("Задача отправлена. ID: ");
    }
}
