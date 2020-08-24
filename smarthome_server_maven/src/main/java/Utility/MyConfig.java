package Utility;

public class MyConfig {
    static String userNameMQTT = "";
    static String passwordMQTT = "";
    static int qos = 2;
    static String broker = "tcp://192.168.1.15:1883";
    static String clientId = "JavaServer";
    static String testChannel = "testChannel";

    public static String getUserNameMQTT() {
        return userNameMQTT;
    }

    public static String getPasswordMQTT() {
        return passwordMQTT;
    }

    public static int getQos() {
        return qos;
    }

    public static String getBroker() {
        return broker;
    }

    public static String getClientId() {
        return clientId;
    }

    public static String getTestChannel() {
        return testChannel;
    }
}
