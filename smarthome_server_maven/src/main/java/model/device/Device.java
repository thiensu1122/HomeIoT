package model.device;

import MQTT.DeviceStatusCode;
import Utility.Utility;

import java.util.Date;
import java.util.List;

public class Device {
    int device_id;
    String name;
    String description;
    float value1;
    float value2;
    byte[] value3;
    Date last_send;
    int status;
    int device_code;
    boolean is_connected;

    public void printDevice(){
        Utility.printOut("Device_id : " + device_id + " name : " + name + " description: " + description + " device_code : " + device_code + " status : " + status+  " value1 : " + value1 + " value2 : " + value2 + " value3 : " + value3);
    }

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

    public int getStatus() {
        return status;
    }

    public void setStatus(int status) {
        if(status == DeviceStatusCode.OFFLINE.getCode())
            is_connected = false;
        this.status = status;
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

    public byte[] getValue3() {
        return value3;
    }

    public void setValue3(byte[] value3) {
        this.value3 = value3;
    }

    public Date getLast_send() {
        return last_send;
    }

    public void setLast_send(Date last_send) {
        this.last_send = last_send;
    }

    public int getDevice_code() {
        return device_code;
    }

    public void setDevice_code(int device_code) {
        this.device_code = device_code;
    }

    public boolean getIs_connected() {
        return is_connected;
    }

    public void setIs_connected(boolean is_connected) {
        this.is_connected = is_connected;
    }

}
