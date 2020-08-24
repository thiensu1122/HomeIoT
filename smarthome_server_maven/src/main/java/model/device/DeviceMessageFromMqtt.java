package model.device;

import model.MessageFromMqtt;

import java.util.List;

public class DeviceMessageFromMqtt extends MessageFromMqtt {
    List<Device> deviceList;

    public List<Device> getDeviceList() {
        return deviceList;
    }


    public void setDeviceList(List<Device> deviceList) {
        this.deviceList = deviceList;
    }
}
