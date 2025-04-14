package vkr.zelenev.ode_systems;

import org.springframework.web.socket.*;
import org.springframework.web.socket.handler.TextWebSocketHandler;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import java.util.*;
import java.util.concurrent.atomic.AtomicInteger;

import org.springframework.stereotype.Component;
import org.springframework.amqp.core.Message;
import org.springframework.beans.factory.annotation.Value;

import io.micrometer.core.instrument.MeterRegistry;
import io.micrometer.core.instrument.Gauge;

@Component
public class SolutionWebSocketHandler extends TextWebSocketHandler {
    
    private static final String TASK_ID_KEY = "taskId";
    private final ObjectMapper objectMapper;
    private final Set<WebSocketSession> sessions = Collections.synchronizedSet(new HashSet<>());
    private final Map<String, Set<WebSocketSession>> taskSubscriptions = Collections.synchronizedMap(new HashMap<>());
  //  private final AtomicInteger activeConnections = new AtomicInteger(0);

    public SolutionWebSocketHandler(ObjectMapper objectMapper/* , MeterRegistry meterRegistry*/) {
        this.objectMapper = objectMapper;
     //   this.activeConnections = meterRegistry.counter("websocket_active_connections");
    }

    @Override
    public void afterConnectionEstablished(WebSocketSession session) {
        sessions.add(session);
    //    activeConnections.incrementAndGet(); 
    }

    @Override
    public void afterConnectionClosed(WebSocketSession session, CloseStatus status) {
        sessions.remove(session);
    //    activeConnections.decrementAndGet();
    }

    @Override
    protected void handleTextMessage(WebSocketSession session, TextMessage message) throws Exception {
        JsonNode json = objectMapper.readTree(message.getPayload());
         
        if (json.has(TASK_ID_KEY)) {
            String taskId = json.get(TASK_ID_KEY).asText();
            taskSubscriptions.computeIfAbsent(taskId, k -> Collections.synchronizedSet(new HashSet<>())).add(session);
        }
    }

    // 🔹 Автоматически получаем ВСЕ сообщения из RabbitMQ в виде `Message` объекта
    @RabbitListener(queues = "${queues.responseQueueName}")
    public void sendUpdateToSubscribers(Message message) {
        try {
            // ✅ Декодируем тело сообщения в строку
            String jsonString = new String(message.getBody());
            // ✅ Парсим JSON
            JsonNode json = objectMapper.readTree(jsonString);

            if (!json.has(TASK_ID_KEY)) {
                System.err.println("❌ Ошибка: В JSON нет taskId! " + jsonString);
                return;
            }

            String taskId = json.get(TASK_ID_KEY).asText();
            Set<WebSocketSession> sessions = taskSubscriptions.get(taskId);
            if (sessions != null) {
                for (WebSocketSession session : sessions) {
                    session.sendMessage(new TextMessage(jsonString));
                }
            }

            // ✅ Выводим мета-информацию из RabbitMQ
            //System.out.println("✅ Получено сообщение из RabbitMQ:");
            //System.out.println("Headers: " + message.getMessageProperties().getHeaders());
            //System.out.println("Content-Type: " + message.getMessageProperties().getContentType());

        } catch (Exception e) {
            System.err.println("❌ Ошибка при обработке сообщения из RabbitMQ: " + e.getMessage());
            e.printStackTrace();
        }
    }
    // // Слушаем очередь ответов от C++ сервера и пересылаем клиенту
    // @RabbitListener(queues = "responseQueue")
    // public void sendUpdateToClients(Message message) {
        
    //     String jsonString = new String(message.getBody());
        
    //     synchronized (sessions) {
    //         for (WebSocketSession session : sessions) {
    //             try 
    //             {              
    //                 session.sendMessage(new TextMessage(jsonString));       
    //             }
    //             catch (Exception e) {
    //                 e.printStackTrace();
    //             }
    //         }
    //     }
    // }
}
