package com.example.androidsmarthome.Activities;

import androidx.appcompat.app.AppCompatActivity;
import androidx.lifecycle.Observer;
import androidx.lifecycle.ViewModelProviders;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

import com.example.androidsmarthome.R;
import com.example.androidsmarthome.Services.MqttService;
import com.example.androidsmarthome.ViewModel.MainActivityViewModel;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "MainActivity";

    private MqttService mqttService;
    private MainActivityViewModel mainActivityViewmodel;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mainActivityViewmodel = ViewModelProviders.of(this).get(MainActivityViewModel.class);
        mainActivityViewmodel.getBinder().observe(this, mMQTTObserver);
    }

    Observer<MqttService.MQTTBinder> mMQTTObserver = new Observer<MqttService.MQTTBinder>() {
        @Override
        public void onChanged(MqttService.MQTTBinder mqttBinder) {
            if(mqttBinder != null){
                Log.d(TAG, "onChanged: "+ "Connected to service");
                mqttService = mqttBinder.getService();
            } else {
                Log.d(TAG, "onChanged: "+ "Unbind from service");
                mqttService = null;
            }
        }
    };

    @Override
    protected void onResume() {
        super.onResume();
        startService();
    }
    private void startService(){
        Intent serviceIntent = new Intent(this, MqttService.class);
        startService(serviceIntent);

        bindService();
    }

    private void bindService(){
        Intent serviceIntent = new Intent(this, MqttService.class);
        bindService(serviceIntent, mainActivityViewmodel.getServiceConnection(), Context.BIND_AUTO_CREATE);
    }

    @Override
    protected void onStop() {
        super.onStop();
        unbindService(mainActivityViewmodel.getServiceConnection());
    }
}