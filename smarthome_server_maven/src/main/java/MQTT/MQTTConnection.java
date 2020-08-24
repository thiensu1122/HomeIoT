package MQTT;

import Utility.MyConfig;
import micro_services.MessageService;
import model.MessageFromMqtt;
import org.eclipse.paho.client.mqttv3.*;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;



public class MQTTConnection {

    MemoryPersistence persistence = new MemoryPersistence() ;
    MqttClient sampleClient;

    public void connect(){
        try {
            sampleClient = new MqttClient( MyConfig.getBroker(), MyConfig.getClientId(), persistence);
            MqttConnectOptions connOpts = new MqttConnectOptions();
            connOpts.setCleanSession(true);
            connOpts.setUserName(MyConfig.getUserNameMQTT());
            connOpts.setPassword(MyConfig.getPasswordMQTT().toCharArray());

            sampleClient.setCallback(new MqttCallback() {

                @Override
                public void connectionLost(Throwable cause) { //Called when the client lost the connection to the broker
                    onConnectionLost(cause);
                }

                @Override
                public void messageArrived(String topic, MqttMessage message) throws Exception {
                    onMessageArrived(topic, message);
                }

                @Override
                public void deliveryComplete(IMqttDeliveryToken token) {//Called when a outgoing publish is complete
                    onMessageDeliveryComplete(token);
                }
            });

            sampleClient.connect(connOpts);


        } catch (MqttException me) {
            me.printStackTrace();
        } catch (Exception ex){
            ex.printStackTrace();
        }
    }

    public boolean isConnected(){
        return sampleClient.isConnected();
    }

    public void publicMessage(String topic, String content) {
        try {
            MqttMessage message = new MqttMessage(content.getBytes());
            message.setQos(MyConfig.getQos());

            sampleClient.publish(topic, message);
        } catch (MqttException e) {
            e.printStackTrace();
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }


    private void onMessageArrived(String topic, MqttMessage message){
        try {
            MessageService messageService = new MessageService(topic, message.toString(), this);
            messageService.start();
        } catch (Exception ex){
            ex.printStackTrace();
        }
    }



    public void subscribeChannel(String channel){
        try {
            //Utility.printOut(channel);
            sampleClient.subscribe(channel);
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    private void onConnectionLost(Throwable cause){

    }

    private void onMessageDeliveryComplete(IMqttDeliveryToken token){

    }


}
