package model.sensor;

import model.MessageFromMqtt;
import model.sensor.Sensor;

import java.util.List;

public class SensorMessageFromMqtt extends MessageFromMqtt {
    List<Sensor> sensorList;

    public List<Sensor> getSensorList() {
        return sensorList;
    }

    public void setSensorList(List<Sensor> sensorList) {
        this.sensorList = sensorList;
    }

}
