import MQTT.MQTTConnection;
import Utility.Utility;
import database.DatabaseConnection;
import model.Android;
import model.Hub;
import model.User;

import java.util.List;

public class Main {
    private static MQTTConnection mqttConnection;
    public static void main(String[] args) {
       List<User> users = getAllUsers();
       mqttConnection = new MQTTConnection();
       mqttConnection.connect();
       subscribeChannels(users);
    }

    private static void subscribeChannels(List<User> users){
        for(User user : users){
            //Utility.printOut(user.getUser_id());
//            List<Hub> hubs = getAllHubsFromUser(user);
//            for(Hub hub : hubs){
//                mqttConnection.subscribeChannel(Utility.getChannelHubPrefix() + hub.getHub_id());
//            }
//            List<Android> androids= getAllAndroidsFromUser(user);
//            for(Android android : androids){
//                mqttConnection.subscribeChannel(Utility.getChannleAndroidPrefix()+ android.getAndroid_id());
//            }
            mqttConnection.subscribeChannel(Utility.getChannelPrefix()+user.getUser_id());
        }
    }

    private static List<Android> getAllAndroidsFromUser(User user){
        DatabaseConnection databaseConnection = new DatabaseConnection();
        List<Android> androids = databaseConnection.getAndroidsfromUser(user);
        return androids;
    }

    private static List<Hub> getAllHubsFromUser(User user){
        DatabaseConnection databaseConnection = new DatabaseConnection();
        List<Hub> hubs = databaseConnection.getHubfromUser(user);
        return hubs;
    }

    private static List<User> getAllUsers(){
        DatabaseConnection databaseConnection = new DatabaseConnection();
        List<User> users = databaseConnection.getAllUser();
        return users;
    }

}
