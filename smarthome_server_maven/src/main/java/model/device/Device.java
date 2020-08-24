package model.device;

import java.util.Date;

public class Device {
    int device_id;
    String name;
    String description;
    float value1;
    float value2;
    String value3;
    Date last_send;
    String status;
    String is_connected;

    public int getDevice_id() {
        return device_id;
    }

    public void setDevice_id(int device_id) {
        this.device_id = device_id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public float getValue1() {
        return value1;
    }

    public void setValue1(float value1) {
        this.value1 = value1;
    }

    public float getValue2() {
        return value2;
    }

    public void setValue2(float value2) {
        this.value2 = value2;
    }

    public String getValue3() {
        return value3;
    }

    public void setValue3(String value3) {
        this.value3 = value3;
    }

    public Date getLast_send() {
        return last_send;
    }

    public void setLast_send(Date last_send) {
        this.last_send = last_send;
    }

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
    }

    public String getIs_connected() {
        return is_connected;
    }

    public void setIs_connected(String is_connected) {
        this.is_connected = is_connected;
    }
}
