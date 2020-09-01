package MQTT;

public enum MessageCode {

    USERREGISTER(0, "Register new user"),
    LOGIN(1, "Check Login"),

    UPDATEALLSENSOR(50,"all sensors data"),
    UPDATESINGLESENSOR(51, "updaate value for single sensor"),
    REQUESTALLSENSORSSTATUS(52, "Request all sensor status"),
    REQUESTSINGLESENSORSTATUS(53, "Request all sensor status"),

    UPDATEALLDEVICESSTATUS(100,"All devices status"),
    UPDATESINGLEDEVICESTATUS(101,"All devices status"),
    REQUESTALLDEVICESTATUS(102,"Request all device status"),
    REQUESTSINGLEDEVICESTATUS(103,"Request all device status"),

    CONTROLDEVICE(200, "Control device"),

    CONFIRM(500, "Confirm message Arrive"),
    RESPONES(501, "response request from android or hub"),
    EMEGENCY(502, "When device find emegency situation")
    ;



    private final int code;
    private final String description;

    private MessageCode(int code, String description) {
        this.code = code;
        this.description = description;
    }

    public String getDescription() {
        return description;
    }

    public int getCode() {
        return code;
    }

    @Override
    public String toString() {
        return code + ": " + description;
    }

}
