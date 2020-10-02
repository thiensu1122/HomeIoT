package com.example.androidsmarthome.Models;

import org.eclipse.paho.client.mqttv3.MqttMessage;

public class MqttPackage {
    MqttMessage message;
    String topic;

    public MqttPackage( String topic, MqttMessage message) {
        this.message = message;
        this.topic = topic;
    }

    public MqttMessage getMessage() {
        return message;
    }

    public void setMessage(MqttMessage message) {
        this.message = message;
    }

    public String getTopic() {
        return topic;
    }

    public void setTopic(String topic) {
        this.topic = topic;
    }
}
