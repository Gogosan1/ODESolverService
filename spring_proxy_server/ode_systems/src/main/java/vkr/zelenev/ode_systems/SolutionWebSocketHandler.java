// import org.springframework.amqp.rabbit.core.RabbitTemplate;
// import org.springframework.web.socket.*;
// import org.springframework.web.socket.handler.TextWebSocketHandler;
// import org.springframework.amqp.rabbit.annotation.RabbitListener;
// import com.fasterxml.jackson.databind.ObjectMapper;
// import java.util.*;

// public class SolutionWebSocketHandler extends TextWebSocketHandler {
//     private final RabbitTemplate rabbitTemplate;
//     private final ObjectMapper objectMapper;
//     private final Set<WebSocketSession> sessions = Collections.synchronizedSet(new HashSet<>());

//     public SolutionWebSocketHandler(RabbitTemplate rabbitTemplate, ObjectMapper objectMapper) {
//         this.rabbitTemplate = rabbitTemplate;
//         this.objectMapper = objectMapper;
//     }

//     @Override
//     public void afterConnectionEstablished(WebSocketSession session) {
//         sessions.add(session);
//     }

//     @Override
//     public void afterConnectionClosed(WebSocketSession session, CloseStatus status) {
//         sessions.remove(session);
//     }

//     // Принимаем JSON от клиента и отправляем в RabbitMQ
//     @Override
//     protected void handleTextMessage(WebSocketSession session, TextMessage message) throws Exception {
//         String jsonMessage = message.getPayload();
//         rabbitTemplate.convertAndSend("cppQueue", jsonMessage);
//         System.out.println("Отправлено в RabbitMQ: " + jsonMessage);
//     }

//     // Слушаем очередь ответов от C++ сервера и пересылаем клиенту
//     @RabbitListener(queues = "responseQueue")
//     public void sendUpdateToClients(String message) {
//         synchronized (sessions) {
//             for (WebSocketSession session : sessions) {
//                 try {
//                     session.sendMessage(new TextMessage(message));
//                 } catch (Exception e) {
//                     e.printStackTrace();
//                 }
//             }
//         }
//     }
// }
