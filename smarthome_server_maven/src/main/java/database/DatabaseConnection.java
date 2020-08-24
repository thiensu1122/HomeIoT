package database;

import Utility.Utility;
import model.Android;
import model.Hub;
import model.device.Device;
import model.device.DeviceMessageFromMqtt;
import model.sensor.Sensor;
import model.User;
import model.sensor.SensorMessageFromMqtt;

import java.sql.*;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class DatabaseConnection {
    private Statement statement = null;
    private ResultSet resultSet = null;

    public Connection getConnection(){
        try {
            return DriverManager.getConnection("jdbc:mysql://localhost/smarthome_iot?user=root&password=1234");
        } catch (SQLException throwables) {
            throwables.printStackTrace();
            return null;
        }
    }

    private int updateDatabase(Connection connect, PreparedStatement preparedStatement){
        int result = 0;
        try {
            // Setup the connection with the DB
            result = preparedStatement.executeUpdate();
        } catch (Exception e){
            e.printStackTrace();
        } finally {
            //close the conenctions no matter what happen.
            closeConnection(connect);
            return result;
        }
    }

    private List<Map<String, Object>> readDataBase(Connection connect, PreparedStatement preparedStatement) {
        // perform read from database here
        resultSet = null;
        List<Map<String, Object>> rows = new ArrayList<Map<String, Object>>();
        try {
            // Setup the connection with the DB
            resultSet = preparedStatement.executeQuery();
            ResultSetMetaData resultSetMetaData = resultSet.getMetaData();
            final int columnCount = resultSetMetaData.getColumnCount();
            while (resultSet.next()) {
                Map<String, Object> row = new HashMap<String, Object>(resultSetMetaData.getColumnCount());

                //column start from 1.
                for (int i = 1; i <= columnCount; ++i) {
                    //resultSetMetaData.getColumnName()
                    row.put(resultSetMetaData.getColumnName(i), resultSet.getObject(i));
                }
                rows.add(row);
            }

        } catch (Exception e){
            e.printStackTrace();
        } finally {
            //close the conenctions no matter what happen.
            closeConnection(connect);
            return rows;
        }
    }

    private void closeConnection(Connection connect){
        try{
            if (resultSet != null) {
                resultSet.close();
            }

            if (statement != null) {
                statement.close();
            }

            if (connect != null) {
                connect.close();
            }
        } catch (Exception e){
            e.printStackTrace();
        }
    }

    public List<User> getAllUser(){
        List<User> users = new ArrayList<>();
        try {

            Connection con = getConnection();
            if(con != null) {
                PreparedStatement preparedStatement = con.prepareStatement("select * from User");

                List<Map<String, Object>> listUsers = readDataBase(con, preparedStatement);
                for (int i = 0; i < listUsers.size(); i++) {
                    User user = new User();
                    Map<String, Object> mapuser = listUsers.get(i);

                    user.setUser_id(Integer.parseInt(mapuser.get("user_id").toString()));
                    if (mapuser.get("status") != null) {
                        user.setStatus(mapuser.get("status").toString());
                    }
                    if (mapuser.get("name") != null) {
                        user.setName(mapuser.get("name").toString());
                    }
                    if (mapuser.get("email") != null) {
                        user.setEmail(mapuser.get("email").toString());
                    }
                    users.add(user);
                }
            }
        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }
        return users;
    }

    public List<Hub> getHubfromUser(User user){
        List<Hub> hubs = new ArrayList<>();
        try {
            Connection con = getConnection();
            if(con != null) {
                PreparedStatement preparedStatement = con.prepareStatement("select * from hub where user_id = ?");
                preparedStatement.setInt(1, user.getUser_id());
                List<Map<String, Object>> listHub = readDataBase(con, preparedStatement);
                for (int i = 0; i < listHub.size(); i++) {
                    Hub hub = new Hub();
                    Map<String, Object> maphub = listHub.get(i);

                    hub.setHub_id(Integer.parseInt(maphub.get("hub_id").toString()));
                    if (maphub.get("user_id") != null) {
                        hub.setUser_id(Integer.parseInt(maphub.get("user_id").toString()));
                    }
                    if (maphub.get("name") != null) {
                        hub.setName(maphub.get("name").toString());
                    }
                    if (maphub.get("location") != null) {
                        hub.setLocation(maphub.get("location").toString());
                    }
                    hubs.add(hub);
                }
            }
        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }
        return hubs;
    }

    public List<Android> getAndroidsfromUser(User user){
        List<Android> androids = new ArrayList<>();
        try {
            Connection con = getConnection();
            if(con != null) {
                PreparedStatement preparedStatement = con.prepareStatement("select * from android where user_id = ?");
                preparedStatement.setInt(1, user.getUser_id());
                List<Map<String, Object>> listAndroid = readDataBase(con, preparedStatement);
                for (int i = 0; i < listAndroid.size(); i++) {
                    Android android = new Android();
                    Map<String, Object> mapAndroid = listAndroid.get(i);

                    android.setAndroid_id(Integer.parseInt(mapAndroid.get("android_id").toString()));
                    if (mapAndroid.get("user_id") != null) {
                        android.setUser_id(Integer.parseInt(mapAndroid.get("user_id").toString()));
                    }
                    if (mapAndroid.get("name") != null) {
                        android.setName(mapAndroid.get("name").toString());
                    }
                    androids.add(android);
                }
            }
        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }
        return androids;
    }

    public void updateSensorValues(Sensor sensor) {
        try {
            Connection con = getConnection();

            if (con != null) {
                PreparedStatement preparedStatement = con.prepareStatement(
                        "UPDATE sensor SET VALUE1=?,VALUE2=?,VALUE3=?,last_updated=NOW(),is_connected=?  WHERE sensor_id = ?");
                preparedStatement.setFloat(1, sensor.getValue1());
                preparedStatement.setFloat(2, sensor.getValue2());
                preparedStatement.setString(3, sensor.getValue3());
                preparedStatement.setBoolean(4, true);
                preparedStatement.setInt(5,sensor.getSensor_id());

                updateDatabase(con, preparedStatement);
            }
        } catch (Exception e){
            e.printStackTrace();
        }
    }

    public void updateDeviceValues(Device device) throws SQLException {
            Connection con = getConnection();
            if (con != null) {
                PreparedStatement preparedStatement = con.prepareStatement(
                        "UPDATE device SET STATUS =?, VALUE1=?,VALUE2=?,VALUE3=?,last_updated=NOW(),is_connected=?  WHERE device_id= ?");
                preparedStatement.setString(1, device.getStatus());
                preparedStatement.setFloat(2, device.getValue1());
                preparedStatement.setFloat(3, device.getValue2());
                preparedStatement.setString(4, device.getValue3());
                preparedStatement.setBoolean(5, true);
                preparedStatement.setInt(6, (device.getDevice_id()));
                preparedStatement.executeUpdate();
            }
    }

    public List<Sensor> getAllSensorsStatus(SensorMessageFromMqtt sensorMessageFromMqtt) throws SQLException {
        List<Sensor> sensors = new ArrayList<>();
        Connection con = getConnection();
        if(con!= null){
            PreparedStatement preparedStatement = con.prepareStatement(
                    "SELECT * FROM SENSOR WHERE hub_id = ? and user_id = ?");
            preparedStatement.setInt(1, sensorMessageFromMqtt.getHub_id());
            preparedStatement.setInt(2, sensorMessageFromMqtt.getUser_id());
            List<Map<String, Object>> listSensors = readDataBase(con, preparedStatement);
            for(int i = 0 ; i < listSensors.size() ;i++){
                Sensor sensor = new Sensor();
                Map<String, Object> mapsensor = listSensors.get(i);

                sensor.setSensor_id(Integer.parseInt(mapsensor.get("sensor_id").toString()));
                if (mapsensor.get("name") != null) {
                    sensor.setName(mapsensor.get("name").toString());
                }
                if(mapsensor.get("description") != null){
                    sensor.setDescription(mapsensor.get("description").toString());
                }
                if(mapsensor.get("value1") != null){
                    sensor.setValue1(Float.parseFloat(mapsensor.get("value1").toString()));
                }
                if(mapsensor.get("value2") != null){
                    sensor.setValue2(Float.parseFloat(mapsensor.get("value2").toString()));
                }
                if(mapsensor.get("value3") != null){
                    sensor.setValue3((mapsensor.get("value3").toString()));
                }
                if(mapsensor.get("is_connected") != null){
                    sensor.setIs_connected(mapsensor.get("is_connected").toString());
                }
                sensors.add(sensor);
            }

        }
        return sensors;
    }

    public Sensor getSingleSensorStatus(SensorMessageFromMqtt sensorMessageFromMqtt) throws SQLException {
        Sensor sensor = new Sensor();
        Connection con = getConnection();
        if(con!= null){
            PreparedStatement preparedStatement = con.prepareStatement(
                    "SELECT * FROM SENSOR WHERE sensor_id = ?");
            preparedStatement.setInt(1, sensorMessageFromMqtt.getSensorList().get(0).getSensor_id());
            List<Map<String, Object>> listSensors = readDataBase(con, preparedStatement);
            for(int i = 0 ; i < listSensors.size() ;i++){
                Map<String, Object> mapsensor = listSensors.get(i);

                sensor.setSensor_id(Integer.parseInt(mapsensor.get("sensor_id").toString()));
                if (mapsensor.get("name") != null) {
                    sensor.setName(mapsensor.get("name").toString());
                }
                if(mapsensor.get("description") != null){
                    sensor.setDescription(mapsensor.get("description").toString());
                }
                if(mapsensor.get("value1") != null){
                    sensor.setValue1(Float.parseFloat(mapsensor.get("value1").toString()));
                }
                if(mapsensor.get("value2") != null){
                    sensor.setValue2(Float.parseFloat(mapsensor.get("value2").toString()));
                }
                if(mapsensor.get("value3") != null){
                    sensor.setValue3((mapsensor.get("value3").toString()));
                }
                if(mapsensor.get("is_connected") != null){
                    sensor.setIs_connected(mapsensor.get("is_connected").toString());
                }
            }
        }
        return sensor;
    }

    public List<Device> getAllDevicesStatus(DeviceMessageFromMqtt deviceMessageFromMqtt) throws SQLException {
        List<Device> devices = new ArrayList<>();
        Connection con = getConnection();
        if(con!= null){
            PreparedStatement preparedStatement = con.prepareStatement(
                    "SELECT * FROM DEVICE WHERE hub_id = ? and user_id = ?");
            preparedStatement.setInt(1, deviceMessageFromMqtt.getHub_id());
            preparedStatement.setInt(2, deviceMessageFromMqtt.getUser_id());
            List<Map<String, Object>> listDevices = readDataBase(con, preparedStatement);
            for(int i = 0 ; i < listDevices.size() ;i++){
                Device device = new Device();
                Map<String, Object> mapdevice = listDevices.get(i);

                device.setDevice_id(Integer.parseInt(mapdevice.get("device_id").toString()));
                if (mapdevice.get("name") != null) {
                    device.setName(mapdevice.get("name").toString());
                }
                if(mapdevice.get("description") != null){
                    device.setDescription(mapdevice.get("description").toString());
                }
                if(mapdevice.get("status") != null){
                    device.setStatus(mapdevice.get("status").toString());
                }
                if(mapdevice.get("value1") != null){
                    device.setValue1(Float.parseFloat(mapdevice.get("value1").toString()));
                }
                if(mapdevice.get("value2") != null){
                    device.setValue2(Float.parseFloat(mapdevice.get("value2").toString()));
                }
                if(mapdevice.get("value3") != null){
                    device.setValue3((mapdevice.get("value3").toString()));
                }
                if(mapdevice.get("is_connected") != null){
                    device.setIs_connected(mapdevice.get("is_connected").toString());
                }
                devices.add(device);
            }

        }
        return devices;
    }

    public Device getSingleDeviceStatus(DeviceMessageFromMqtt deviceMessageFromMqtt) throws SQLException {
        Device device = new Device();
        Connection con = getConnection();
        if(con!= null){
            PreparedStatement preparedStatement = con.prepareStatement(
                    "SELECT * FROM DEVICE WHERE device_id = ?");
            preparedStatement.setInt(1, deviceMessageFromMqtt.getDeviceList().get(0).getDevice_id());
            List<Map<String, Object>> listSensors = readDataBase(con, preparedStatement);
            for(int i = 0 ; i < listSensors.size() ;i++){
                Map<String, Object> mapdevice = listSensors.get(i);

                device.setDevice_id(Integer.parseInt(mapdevice.get("device_id").toString()));
                if (mapdevice.get("name") != null) {
                    device.setName(mapdevice.get("name").toString());
                }
                if(mapdevice.get("description") != null){
                    device.setDescription(mapdevice.get("description").toString());
                }
                if(mapdevice.get("status") != null){
                    device.setStatus(mapdevice.get("status").toString());
                }
                if(mapdevice.get("value1") != null){
                    device.setValue1(Float.parseFloat(mapdevice.get("value1").toString()));
                }
                if(mapdevice.get("value2") != null){
                    device.setValue2(Float.parseFloat(mapdevice.get("value2").toString()));
                }
                if(mapdevice.get("value3") != null){
                    device.setValue3((mapdevice.get("value3").toString()));
                }
                if(mapdevice.get("is_connected") != null){
                    device.setIs_connected(mapdevice.get("is_connected").toString());
                }
            }
        }
        return device;
    }
}
