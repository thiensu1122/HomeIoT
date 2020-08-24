package micro_services;

import MQTT.MQTTConnection;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.lang.reflect.Method;

import static org.junit.jupiter.api.Assertions.*;

class MessageServiceTest {
    MQTTConnection mqttConnection;
    @BeforeEach
    void setUp() {
        mqttConnection.connect();
    }

    @Test
    void test() {
        MessageService messageService = new MessageService("test", "test", mqttConnection);

    }
}