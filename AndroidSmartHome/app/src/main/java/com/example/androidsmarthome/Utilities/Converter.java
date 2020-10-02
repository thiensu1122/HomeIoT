package com.example.androidsmarthome.Utilities;

import com.example.androidsmarthome.Models.MessageMqttFrameUser;
import com.example.androidsmarthome.Models.User;
import com.google.gson.Gson;

public class Converter {
    static Gson gson = new Gson();

    public static String messageMqttFrameUsertoString(MessageMqttFrameUser messageFrame) {
        String json = gson.toJson(messageFrame);
        return json;
    }
    public static User userFromMessageMqttFrame(String messageMqtt){
        User user = gson.fromJson(messageMqtt, User.class);
        return user;

    }
}
