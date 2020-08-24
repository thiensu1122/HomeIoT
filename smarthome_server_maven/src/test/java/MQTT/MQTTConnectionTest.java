package MQTT;

import Utility.MyConfig;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.time.Duration;

import static org.junit.jupiter.api.Assertions.*;

class MQTTConnectionTest {
    MQTTConnection mqttConnection;

    @BeforeEach
    void setUp() {
        mqttConnection = new MQTTConnection();
        mqttConnection.connect();
    }

    @Test
    void connectTest() {
        assertTrue(mqttConnection.isConnected());
    }

    @Test
    void subscribeChannelTest() {

        mqttConnection.subscribeChannel(MyConfig.getTestChannel());

    }

}