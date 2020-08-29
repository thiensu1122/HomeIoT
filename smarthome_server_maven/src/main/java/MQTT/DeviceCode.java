package MQTT;

public enum DeviceCode {
    DHT11(0, "DHT11 measure temp and humi"),
    RGBLED(1, "Red green blue led");
    int code;
    int description;

    DeviceCode(int i, String s) {
    }

    public int getCode() {
        return code;
    }

    public int getDescription() {
        return description;
    }
}
