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
    public ResponseEntity<String> sendTask(@RequestBody Map<String, Object> jsonMessage) {

        //String taskID = UUID.randomUUID().toString();
        //jsonMessage.put("taskID", taskID);
        
        rabbitTemplate.convertAndSend("cppQueue", jsonMessage);

        return ResponseEntity.ok("Задача отправлена. ID: ");

        // Message response = rabbitTemplate.receive("responseQueue", 100000);

        // if (response != null) {
        //     try {
        //         // Конвертируем строку JSON в Map
        //         String jsonString = new String(response.getBody());
        //         ObjectMapper objectMapper = new ObjectMapper();
        //         return objectMapper.readValue(jsonString, Map.class);
        //     } catch (Exception e) {
        //         return Map.of("error", "Ошибка парсинга JSON", "details", e.getMessage());
        //     }
        // } else {
        //     return Map.of("error", "Ответ не получен");
        // }
    }
}
