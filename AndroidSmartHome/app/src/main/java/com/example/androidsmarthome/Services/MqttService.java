package com.example.androidsmarthome.Services;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.Binder;
import android.os.Handler;
import android.os.IBinder;
import android.util.Log;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;

import com.example.androidsmarthome.Models.MqttPackage;
import com.example.androidsmarthome.Repositories.MQTTStage;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttMessageListener;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

public class MqttService extends Service {
    public static final String TAG = "MQTT Service";

    String clientId = MqttClient.generateClientId();
    Context application;
    MqttAndroidClient client = new MqttAndroidClient(application, "BROKER SERVER", clientId);

    MutableLiveData<MQTTStage> mqttStage = new MutableLiveData<>();
    MutableLiveData<MqttPackage> mutableMqttPackage = new MutableLiveData<>();
    private IBinder mBinder = new MQTTBinder();
    private Handler mhandler;

    @Override
    public void onCreate() {
        super.onCreate();
        try {
            client.connect().setActionCallback(onMqttConnect);
            client.disconnect().setActionCallback(onMqttDisconnect);
        }catch (Exception e){
            Log.e(TAG, "onCreate: " + e.getMessage());
            e.printStackTrace();
        }
    }

    public MqttService() {

    }

    public LiveData<MqttPackage> getMqttPackage(){
        return mutableMqttPackage;
    }
    public void connect(){
        try {
            if(!client.isConnected()){
                client.connect().setActionCallback(onMqttConnect);
            }
        } catch (Exception e){
            Log.e(TAG, "connect: ");
            e.printStackTrace();
        }
    }

//    private static MQTTService instance;
//
//    public static MQTTService getInstance(){
//        if(instance ==null){
//            instance = new MQTTService();
//        }
//        return instance;
//    }

    ////run every time someone call startservice.
    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        if(mqttStage.getValue() != MQTTStage.Connected){
            connect();
        }
        return super.onStartCommand(intent, flags, startId);
    }

    @Override
    public IBinder onBind(Intent intent) {
        // TODO: Return the communication channel to the service.
        throw new UnsupportedOperationException("Not yet implemented");
    }

    public class MQTTBinder extends Binder{
        public MqttService getService(){
            return MqttService.this;
        }
    }

    //stop when user completely close app.
    @Override
    public void onTaskRemoved(Intent rootIntent) {
        super.onTaskRemoved(rootIntent);
        stopSelf();
    }

    public void publicMessage(String topic,String payload) throws MqttException {
        MqttMessage message = new MqttMessage(payload.toString().getBytes());
        client.publish(topic, message);
    }

    public void subscribeTopic(String topic, int qos) {
//        try {
//            IMqttToken subToken = client.subscribe(topic, qos, iMqttMessageListener);
//        } catch (Exception e){
//            Log.e(TAG, "subscribeTopic: " + e.getMessage() );
//            e.printStackTrace();
//        }

    }

    IMqttMessageListener iMqttMessageListener = new IMqttMessageListener() {
        @Override
        public void messageArrived(String topic, MqttMessage message) throws Exception {
            MqttPackage mqttPackage = new MqttPackage(topic, message);
            mutableMqttPackage.postValue(mqttPackage);
        }
    };

    public void unSubscribeTopic(String topic) throws MqttException {
        IMqttToken unsubToken = client.unsubscribe(topic);
        unsubToken.setActionCallback(onUnsubscribeTopic);
    }

    IMqttActionListener onMqttConnect = new IMqttActionListener() {
        @Override
        public void onSuccess(IMqttToken asyncActionToken) {
            mqttStage.postValue(MQTTStage.Connected);
        }

        @Override
        public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
            mqttStage.postValue(MQTTStage.ConnecteFail);
        }
    };

    IMqttActionListener onMqttDisconnect = new IMqttActionListener() {
        @Override
        public void onSuccess(IMqttToken asyncActionToken) {
            mqttStage.postValue(MQTTStage.Disconnected);
        }

        @Override
        public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
            mqttStage.postValue(MQTTStage.DisconnectFail);
        }
    };

    IMqttActionListener onUnsubscribeTopic = new IMqttActionListener() {
        @Override
        public void onSuccess(IMqttToken asyncActionToken) {

        }

        @Override
        public void onFailure(IMqttToken asyncActionToken, Throwable exception) {

        }
    };


}
