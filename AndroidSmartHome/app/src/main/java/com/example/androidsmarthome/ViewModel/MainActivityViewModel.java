package com.example.androidsmarthome.ViewModel;

import android.content.ComponentName;
import android.content.ServiceConnection;
import android.os.IBinder;
import android.util.Log;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

import com.example.androidsmarthome.Services.MqttService;

public class MainActivityViewModel extends ViewModel {
    private static final String TAG = "MainActivityViewModel";

    private MutableLiveData<Boolean> mIsProcessUpdating = new MutableLiveData<>();
    private MutableLiveData<MqttService.MQTTBinder> mBinder = new MutableLiveData<>();

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

    public LiveData<Boolean> getIsProcessUpdate(){
        return mIsProcessUpdating;
    }
    public LiveData<MqttService.MQTTBinder> getBinder(){
        return mBinder;
    }

    public ServiceConnection getServiceConnection(){
        return serviceConnection;
    }
}
