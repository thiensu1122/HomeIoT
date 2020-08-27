package micro_services;

import MQTT.DeviceStatusCode;
import MQTT.DirectionCode;
import MQTT.MQTTConnection;
import MQTT.MessageCode;
import Utility.Utility;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.google.gson.Gson;
import com.google.gson.JsonObject;
import database.DatabaseConnection;
import model.ConfirmMessageFromMqtt;
import model.MessageFromMqtt;
import model.device.Device;
import model.device.DeviceMessageFromMqtt;
import model.sensor.Sensor;
import model.sensor.SensorMessageFromMqtt;

import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

public class MessageService implements Runnable {
    int code;
    String topic;
    String message;
    Thread thread;
    MQTTConnection mqttConnection;
    Gson gson = new Gson();
    ObjectMapper mapper = new ObjectMapper();

    public void start(){
        if(thread == null){
            thread = new Thread(this);
            thread.start();
        }
    }

    public MessageService(String topic, String message, MQTTConnection mqttConnection) {
        this.topic = topic;
        this.message = message;
        this.mqttConnection = mqttConnection;
        this.code = getCodeFromMessage(message);
    }
    //get Code from message this message is raw. we need to know code to parse the message correctly.
    private int getCodeFromMessage(String message) {

        JsonObject jsonMessage = gson.fromJson(message, JsonObject.class);
        return jsonMessage.get("code").getAsInt();

    }
    // new thread for every message.
    @Override
    public void run() {
        try {
            Utility.printOut(topic + ": " + message);
            // Handle all the sensor message
            // !!!!!!!!!!! when add new sensor remember to put it to funcion isSensorMessage
//////            if(Utility.isSensorMessage(this.code)){
//////                SensorMessageFromMqtt sensorMqttMessage = gson.fromJson(message, SensorMessageFromMqtt.class);
//////                processMessageSensor(topic, sensorMqttMessage);
//////                // !!!!!!!!!!! when add new device remember to put it to funcion isDeviceMessage
////            } else if(Utility.isDeviceMessage(this.code)){
                DeviceMessageFromMqtt deviceMessageFromMqtt = gson.fromJson(message, DeviceMessageFromMqtt.class);
                if(deviceMessageFromMqtt.getDirectionCode() == DirectionCode.HUBTOSERVER.getCode() || deviceMessageFromMqtt.getDirectionCode() == DirectionCode.ANDROIDTOSERVER.getCode()){
                    processMessageDevice(topic, deviceMessageFromMqtt);
                }
//            }
        } catch (Exception ex){
            ex.printStackTrace();
        }
    }

    private void processMessageDevice(String topic, DeviceMessageFromMqtt deviceMessageFromMqtt) throws SQLException {
        if(MessageCode.UPDATEALLDEVICESSTATUS.getCode() == deviceMessageFromMqtt.getCode()){
            handleUpdateAllDevicesStatus(topic, deviceMessageFromMqtt);
        } else if(MessageCode.UPDATESINGLEDEVICESTATUS.getCode() == deviceMessageFromMqtt.getCode()){
            handleUpdateSingleDeviceStatus(topic, deviceMessageFromMqtt);
        } else if(MessageCode.REQUESTALLDEVICESTATUS.getCode() == deviceMessageFromMqtt.getCode()){
            handleRequestAllDevicesStatus(topic, deviceMessageFromMqtt);
        }else if(MessageCode.REQUESTSINGLEDEVICESTATUS.getCode() == deviceMessageFromMqtt.getCode()){
            handleRequestSingleDeviceStatus(topic, deviceMessageFromMqtt);
        } else if(MessageCode.CONFIRM.getCode() == deviceMessageFromMqtt.getCode() && deviceMessageFromMqtt.getDirectionCode() == (DirectionCode.HUBTOSERVER.getCode())){
            handleConfirm(topic, deviceMessageFromMqtt);
        }
//        sendConfirmMessage(topic, deviceMessageFromMqtt, gson.toJson(deviceMessageFromMqtt));
    }

    private void handleConfirm(String topic, DeviceMessageFromMqtt deviceMessageFromMqtt) throws SQLException {
        DatabaseConnection databaseConnection = new DatabaseConnection();
        List<Device> devices = deviceMessageFromMqtt.getDeviceList();
        databaseConnection.updateDeviceValues(devices.get(0)); //only one device.
    }

    private void handleRequestSingleDeviceStatus(String topic, DeviceMessageFromMqtt deviceMessageFromMqtt) throws SQLException {
        List<Device> devices = getDeviceFromDatabase(deviceMessageFromMqtt);
        sendDeviceMessage(topic, deviceMessageFromMqtt, devices);
    }

    // public for testing purpose
    public List<Device> getDeviceFromDatabase(DeviceMessageFromMqtt deviceMessageFromMqtt) throws SQLException {
        DatabaseConnection databaseConnection = new DatabaseConnection();
        Device device = databaseConnection.getSingleDeviceStatus(deviceMessageFromMqtt);
        List<Device> devices = new ArrayList<>();
        devices.add(device);
        return devices;
    }

    private void handleRequestAllDevicesStatus(String topic, DeviceMessageFromMqtt deviceMessageFromMqtt) throws SQLException {
        List<Device> devices = getAlldevicesFromDatabase(deviceMessageFromMqtt);
        sendDeviceMessage(topic, deviceMessageFromMqtt, devices);
    }
    //public for testing purpose
    public List<Device> getAlldevicesFromDatabase(DeviceMessageFromMqtt deviceMessageFromMqtt) throws SQLException {
        DatabaseConnection databaseConnection = new DatabaseConnection();
        return databaseConnection.getAllDevicesStatus(deviceMessageFromMqtt);
    }

    //public for testing purpose
    public void handleUpdateSingleDeviceStatus(String topic, DeviceMessageFromMqtt deviceMessageFromMqtt) throws SQLException {
        DatabaseConnection databaseConnection = new DatabaseConnection();
        List<Device> devices = deviceMessageFromMqtt.getDeviceList();
        devices.get(0).setIs_connected(devices.get(0).getStatus() != DeviceStatusCode.OFFLINE.getCode());
        databaseConnection.updateDeviceValues(devices.get(0)); //only one device.
    }

    //public for testing purpose
    public void handleUpdateAllDevicesStatus(String topic, DeviceMessageFromMqtt deviceMessageFromMqtt) throws SQLException {
        DatabaseConnection databaseConnection = new DatabaseConnection();
        List<Device> devices = deviceMessageFromMqtt.getDeviceList();
        for(Device device : devices){
            device.printDevice();
            device.setIs_connected(device.getStatus() != DeviceStatusCode.OFFLINE.getCode());
            databaseConnection.updateDeviceValues(device);
        }
    }

//    private void processMessageSensor(String topic, SensorMessageFromMqtt sensorMessageFromMqtt) throws JsonProcessingException, SQLException {
//        //due to gson policy we need Sensorlist Class to parse string message to sensorMessageFromMqtt object
//        if(MessageCode.UPDATEALLSENSOR.getCode() == (sensorMessageFromMqtt.getCode())){
//            handleUpdateAllSensorsStatus(topic, sensorMessageFromMqtt);
//        }else if(MessageCode.UPDATESINGLESENSOR.getCode() == sensorMessageFromMqtt.getCode()){
//            handleUpdateSingleSensorStatus(topic, sensorMessageFromMqtt);
//        } else if(MessageCode.REQUESTALLSENSORSSTATUS.getCode() == sensorMessageFromMqtt.getCode()){
//            handleRequestAllSensorStatus(topic, sensorMessageFromMqtt);
//        } else if(MessageCode.REQUESTSINGLESENSORSTATUS.getCode() == sensorMessageFromMqtt.getCode()) {
//            handleRequestSingleSensorStatus(topic, sensorMessageFromMqtt);
//        }
////        sendConfirmMessage(topic, sensorMessageFromMqtt, gson.toJson(sensorMessageFromMqtt));
//    }
//
//    private void handleRequestSingleSensorStatus(String topic, SensorMessageFromMqtt sensorMessageFromMqtt) throws SQLException {
//        List<Sensor> sensors = getSensorsFromDatabase(sensorMessageFromMqtt);
//        sendSensorMessage(topic, sensorMessageFromMqtt, sensors);
//    }
//
//    //public for testing purpose
//    public List<Sensor> getSensorsFromDatabase(SensorMessageFromMqtt sensorMessageFromMqtt) throws SQLException {
//        DatabaseConnection databaseConnection = new DatabaseConnection();
//        Sensor sensor = databaseConnection.getSingleSensorStatus(sensorMessageFromMqtt);
//        List<Sensor> sensors = new ArrayList<>();
//        sensors.add(sensor);
//        //only one sensor however to create response we need to have list.
//        return sensors;
//    }
//
//    private void handleRequestAllSensorStatus(String topic, SensorMessageFromMqtt sensorMessageFromMqtt) throws SQLException {
//        List<Sensor> sensors = getSensorFromDatabase(sensorMessageFromMqtt);
//        sendSensorMessage(topic, sensorMessageFromMqtt, sensors);
//    }
//    //public for testing purpose
//    public List<Sensor> getSensorFromDatabase(SensorMessageFromMqtt sensorMessageFromMqtt) throws SQLException {
//        DatabaseConnection databaseConnection = new DatabaseConnection();
//        return databaseConnection.getAllSensorsStatus(sensorMessageFromMqtt);
//    }
//
//    //public for testing purpose
//    public void handleUpdateSingleSensorStatus(String topic, SensorMessageFromMqtt sensorMessageFromMqtt) {
//        DatabaseConnection databaseConnection = new DatabaseConnection();
//        List<Sensor> sensors = sensorMessageFromMqtt.getSensorList();
//        databaseConnection.updateSensorValues(sensors.get(0)); // only one sensor here.
//    }
//
//    //public for testing purpose
//    public void handleUpdateAllSensorsStatus(String topic, SensorMessageFromMqtt sensorMessageFromMqtt) {
//        DatabaseConnection databaseConnection = new DatabaseConnection();
//        List<Sensor> sensors = sensorMessageFromMqtt.getSensorList();
//        for (Sensor sensor: sensors) {
//            databaseConnection.updateSensorValues(sensor); // update one by one sensor in the list
//        }
//    }
//
//
    private void sendDeviceMessage(String topic, DeviceMessageFromMqtt deviceMessageFromMqtt, List<Device> devices){
        DeviceMessageFromMqtt responseMessage = new DeviceMessageFromMqtt();
        responseMessage = deviceMessageFromMqtt;
        responseMessage.setDeviceList(devices);
        responseMessage.setCode(MessageCode.RESPONES.getCode());
        responseMessage.setDirectionCode(DirectionCode.getReverseDirectionCode(deviceMessageFromMqtt.getDirectionCode()));
        mqttConnection.publicMessage(topic, gson.toJson(responseMessage));
    }
//
//    private void sendSensorMessage(String topic, SensorMessageFromMqtt sensorMessageFromMqtt, List<Sensor> sensors) {
//        SensorMessageFromMqtt responseMessage = new SensorMessageFromMqtt();
//        responseMessage = sensorMessageFromMqtt;
//        responseMessage.setCode(MessageCode.RESPONES.getCode());
//        responseMessage.setSensorList(sensors);
//        responseMessage.setDirectionCode(DirectionCode.getReverseDirectionCode(sensorMessageFromMqtt.getDirectionCode()));
//        mqttConnection.publicMessage(topic, gson.toJson(responseMessage));
//    }
//
//
    private void sendConfirmMessage(String topic, MessageFromMqtt mqttMessage, String data){

        ConfirmMessageFromMqtt confirmMessage = new ConfirmMessageFromMqtt();
        confirmMessage.setCode(MessageCode.CONFIRM.getCode());
        confirmMessage.setDirectionCode(DirectionCode.getReverseDirectionCode(mqttMessage.getDirectionCode()));
        confirmMessage.setConfirmData(data);
        mqttConnection.publicMessage(topic, gson.toJson(confirmMessage));

    }

}
