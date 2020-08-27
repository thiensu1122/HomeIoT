package MQTT;

public enum DeviceStatusCode {
    OFFLINE( 255, "Device is offline");

    private final int code;
    private final String message;

    DeviceStatusCode(int code, String message) {
        this.code = code;
        this.message = message;
    }

    public int getCode() {
        return code;
    }

    public String getMessage() {
        return message;
    }
}
