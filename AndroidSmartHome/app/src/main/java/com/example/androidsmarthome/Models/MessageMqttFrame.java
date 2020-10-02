package com.example.androidsmarthome.Models;


import java.util.Date;

public class MessageMqttFrame {
    int code;
    String message;
    int user_id;
    int hub_id;
    Date receiveDate;
    String from;
    String to;
    int directionCode;

    public void setReceiveDate(){
        receiveDate = new Date();
    }

    public int getCode() {
        return code;
    }

    public void setCode(int code) {
        this.code = code;
    }

    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }

    public int getUser_id() {
        return user_id;
    }

    public void setUser_id(int user_id) {
        this.user_id = user_id;
    }

    public int getHub_id() {
        return hub_id;
    }

    public void setHub_id(int hub_id) {
        this.hub_id = hub_id;
    }

    public Date getReceiveDate() {
        return receiveDate;
    }

    public void setReceiveDate(Date receiveDate) {
        this.receiveDate = receiveDate;
    }

    public String getFrom() {
        return from;
    }

    public void setFrom(String from) {
        this.from = from;
    }

    public String getTo() {
        return to;
    }

    public void setTo(String to) {
        this.to = to;
    }

    public int getDirectionCode() {
        return directionCode;
    }

    public void setDirectionCode(int directionCode) {
        this.directionCode = directionCode;
    }
}
