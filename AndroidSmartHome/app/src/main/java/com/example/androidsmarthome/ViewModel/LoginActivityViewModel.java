package com.example.androidsmarthome.ViewModel;

import android.content.ComponentName;
import android.content.ServiceConnection;
import android.os.IBinder;
import android.util.Log;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

import com.example.androidsmarthome.Models.MqttPackage;
import com.example.androidsmarthome.Models.MessageMqttFrameUser;
import com.example.androidsmarthome.Models.User;
import com.example.androidsmarthome.Services.MqttService;
import com.example.androidsmarthome.Utilities.Converter;
import com.example.androidsmarthome.Utilities.MessageCode;
import com.google.gson.Gson;
import com.google.gson.JsonObject;

public class LoginActivityViewModel extends ViewModel {
    private static final String TAG = "LoginActivityViewModel";

    private MutableLiveData<MqttService.MQTTBinder> mBinder = new MutableLiveData<>();
    private MutableLiveData<Boolean> loginStage = new MutableLiveData<>();
    private User user = null;

    public User getUser() {
        return user;
    }

    public void setUser(User user) {
        this.user = user;
    }

    public void init(){
        loginStage.postValue(false);
    }

    private ServiceConnection serviceConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName componentName, IBinder iBinder) {
            Log.d(TAG, "onServiceConnected: Connnected to service");
            MqttService.MQTTBinder binder = (MqttService.MQTTBinder) iBinder;
            mBinder.postValue(binder);
        }
        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            mBinder.postValue(null);
        }
    };

    public LiveData<MqttService.MQTTBinder> getBinder(){
        return mBinder;
    }

    public ServiceConnection getServiceConnection(){
        return serviceConnection;
    }

    public void checklogin(MqttPackage mqttPackage) {
        JsonObject convertedObject = new Gson().fromJson(mqttPackage.getMessage().toString(), JsonObject.class);
        int code = convertedObject.get("code").getAsInt();
        if(code == MessageCode.LOGINSUCCESS.getCode()) {
            user = Converter.userFromMessageMqttFrame(mqttPackage.getMessage().toString());
            loginStage.postValue(true);
        }
    }

    public String getLoginMqttMessage(String userName, String password) {
        MessageMqttFrameUser messageFrame = new MessageMqttFrameUser();
        messageFrame.setCode(MessageCode.LOGIN.getCode());
        messageFrame.setUserName(userName);
        messageFrame.setPassword(password);
        String message = Converter.messageMqttFrameUsertoString(messageFrame);
        return message;
    }

    public MutableLiveData<Boolean> getLoginStage() {
        return loginStage;
    }

}
