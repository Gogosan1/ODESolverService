package vkr.zelenev.ode_systems;

import java.util.Map;
import java.util.UUID;
import java.util.stream.Stream;


import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.web.bind.annotation.*;

import org.springframework.amqp.core.Message; 

@RestController
@RequestMapping("/messages")
public class LibraryController {

    private final RabbitTemplate rabbitTemplate;

    public LibraryController(RabbitTemplate rabbitTemplate) {
        this.rabbitTemplate = rabbitTemplate;
    }

    @PostMapping("/json")
    public String sendJsonMessage(@RequestBody Map<String, Object> jsonMessage) {
        // Генерируем уникальный correlationId
        String correlationId = UUID.randomUUID().toString();
        //System.out.println(jsonMessage);

        // Отправляем сообщение в `cppQueue`, указывая `responseQueue` для ответа
        jsonMessage.put("correlationId", correlationId);
        rabbitTemplate.convertAndSend("cppQueue", jsonMessage);

        Message response = rabbitTemplate.receive("responseQueue", 100000);
        
        if (response.getBody() != null) {
            return "Response received: " + response;
        } else {
            return "No response received (timeout)";
        }
    }
}
