package model;

public class ConfirmMessageFromMqtt extends MessageFromMqtt {
    String confirmData;

    public String getConfirmData() {
        return confirmData;
    }

    public void setConfirmData(String confirmData) {
        this.confirmData = confirmData;
    }
}
