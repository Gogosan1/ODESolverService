package vkr.zelenev.ode_systems;

import org.springframework.web.socket.*;
import org.springframework.web.socket.handler.TextWebSocketHandler;
import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import com.fasterxml.jackson.databind.ObjectMapper;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.springframework.amqp.core.Message;

import java.util.*;
import io.micrometer.core.instrument.MeterRegistry;
import io.micrometer.core.instrument.Gauge;
import java.util.concurrent.atomic.AtomicInteger;
import io.micrometer.core.instrument.Counter;

@Component
public class SolutionWebSocketHandler extends TextWebSocketHandler {

    private static final String SOLVER_KEY = "solver";
    private static final String SESSION_ID_KEY = "sessionId";
    private static final String SUBSCRIBE_KEY = "subscribe";
    private static final String UNSUBSCRIBE_KEY = "unsubscribe";

    private final ObjectMapper objectMapper;
    private final RabbitTemplate rabbitTemplate;
    private final QueueConfig queueConfig;

    private final Map<String, Set<WebSocketSession>> sessionSubscriptions = new HashMap<>();
    private final Map<String, WebSocketSession> sessionById = new HashMap<>();

    private final AtomicInteger activeConnections = new AtomicInteger(0);
    private final Counter taskSubmissionCounter;

    @Autowired
    public SolutionWebSocketHandler(ObjectMapper objectMapper, RabbitTemplate rabbitTemplate, QueueConfig queueConfig,  MeterRegistry registry) {
        this.objectMapper = objectMapper;
        this.rabbitTemplate = rabbitTemplate;
        this.queueConfig = queueConfig;

        Gauge.builder("websocket_active_connections", activeConnections, AtomicInteger::get)
        .description("Количество активных WebSocket-подключений")
        .register(registry);

         this.taskSubmissionCounter = Counter.builder("websocket_task_submissions_total")
                .description("Общее количество полученных задач через WebSocket")
                .register(registry);
    }

    @Override
    public void afterConnectionEstablished(WebSocketSession session) {
        
        String sessionId = UUID.randomUUID().toString();
        sessionById.put(sessionId, session);
        sessionSubscriptions.computeIfAbsent(sessionId, k -> new HashSet<>()).add(session);
        
        try {
            session.sendMessage(new TextMessage("{\"sessionId\": \"" + sessionId + "\"}"));
        } catch (Exception e) {
            e.printStackTrace();
        }
        activeConnections.incrementAndGet();
    }

    @Override
    public void afterConnectionClosed(WebSocketSession session, CloseStatus status) {
        sessionById.values().remove(session);

        sessionSubscriptions.values().forEach(subscribers -> subscribers.remove(session));
        activeConnections.decrementAndGet();
    }

    @Override
    protected void handleTextMessage(WebSocketSession session, TextMessage message) throws Exception {
        Map<String, Object> jsonMessage = objectMapper.readValue(message.getPayload(), Map.class);
        try{
        if (jsonMessage.containsKey(SUBSCRIBE_KEY)) {
            handleSubscribe(session, jsonMessage);
        } else if (jsonMessage.containsKey(UNSUBSCRIBE_KEY)) {
            handleUnsubscribe(session, jsonMessage);
        } else {
            handleTaskSubmission(session, jsonMessage);
        }
    }
    catch(Exception e)
    {
        System.out.println(e.getMessage());
    }
    }

    private void handleSubscribe(WebSocketSession session, Map<String, Object> jsonMessage) throws Exception {
        String sessionIdToSubscribe = (String) jsonMessage.get(SUBSCRIBE_KEY);
        WebSocketSession targetSession = sessionById.get(sessionIdToSubscribe);
        
        if (targetSession != null && !session.equals(targetSession)) {
            sessionSubscriptions.computeIfAbsent(sessionIdToSubscribe, k -> new HashSet<>()).add(session);
            session.sendMessage(new TextMessage("{\"status\": \"subscribed\", \"sessionId\": \"" + sessionIdToSubscribe + "\"}"));
        } else {
            session.sendMessage(new TextMessage("{\"status\": \"error\", \"message\": \"Cannot subscribe to the same session or invalid sessionId.\"}"));
        }
    }

    private void handleUnsubscribe(WebSocketSession session, Map<String, Object> jsonMessage) throws Exception {
        String sessionIdToUnsubscribe = (String) jsonMessage.get(UNSUBSCRIBE_KEY);

        
        if (!sessionById.containsKey(sessionIdToUnsubscribe)) {
            session.sendMessage(new TextMessage("{\"status\": \"error\", \"message\": \"Invalid sessionId to unsubscribe.\"}"));
            return;
        }

        if (sessionSubscriptions.containsKey(sessionIdToUnsubscribe)) {
            Set<WebSocketSession> subscribers = sessionSubscriptions.get(sessionIdToUnsubscribe);
            if (subscribers.contains(session)) {
                subscribers.remove(session);
                session.sendMessage(new TextMessage("{\"status\": \"unsubscribed\", \"sessionId\": \"" + sessionIdToUnsubscribe + "\"}"));
            } else {
                session.sendMessage(new TextMessage("{\"status\": \"error\", \"message\": \"You are not subscribed to this session.\"}"));
            }
        } else {
            session.sendMessage(new TextMessage("{\"status\": \"error\", \"message\": \"No one is subscribed to this session.\"}"));
        }
    }

    private void handleTaskSubmission(WebSocketSession session, Map<String, Object> jsonMessage) throws Exception {
        String solverType = (String) jsonMessage.get(SOLVER_KEY);
        String sessionId1 = (String) jsonMessage.get(SESSION_ID_KEY);
        if(sessionById.containsKey(sessionId1))
        {
            if (queueConfig.getQueueNames().contains(solverType.toLowerCase())) {
                rabbitTemplate.convertAndSend(solverType.toLowerCase(), jsonMessage);
                session.sendMessage(new TextMessage("\"{\"status\": \"sent\", \"message\": \"Message sent to" + solverType + " solver.\"}"));
            } else {
                session.sendMessage(new TextMessage("{\"status\": \"error\", \"message\": \"There is no solver with" + solverType + " name.\"}"));
            }
        }     
        else
        {
            session.sendMessage(new TextMessage("{\"status\": \"error\", \"message\": \"There is no session with:" + sessionId1 + " id.\"}"));
        }
        taskSubmissionCounter.increment();
    }

    public void sendUpdateToSubscribers(String sessionId, String message) {
        Set<WebSocketSession> subscribers = sessionSubscriptions.get(sessionId);
        if (subscribers != null) {
            for (WebSocketSession subscriber : subscribers) {
                try {
                    subscriber.sendMessage(new TextMessage(message));
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
    }

    @RabbitListener(queues = "${queues.responseQueueName}")
    public void handleResponseQueueMessage(Message message) {
        try {

            String jsonString = new String(message.getBody());
           

            Map<String, Object> jsonMessage = objectMapper.readValue(jsonString, Map.class);
            String sessionId = (String) jsonMessage.get(SESSION_ID_KEY);

            // Отправляем сообщение всем подписчикам сессии
            if (sessionId != null) {
                sendUpdateToSubscribers(sessionId, jsonString);
            } else {
                System.err.println("Ошибка: В сообщении из RabbitMQ отсутствует sessionId.");
            }

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
