package com.example.androidsmarthome.Activities;

import androidx.appcompat.app.AppCompatActivity;
import androidx.lifecycle.Observer;
import androidx.lifecycle.ViewModelProviders;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.example.androidsmarthome.Models.MqttPackage;
import com.example.androidsmarthome.R;
import com.example.androidsmarthome.Services.MqttService;
import com.example.androidsmarthome.Utilities.Variables;
import com.example.androidsmarthome.ViewModel.LoginActivityViewModel;

import org.eclipse.paho.client.mqttv3.MqttException;

public class LoginActivity extends AppCompatActivity {

    private static final String TAG = "LoginActivity";
    private LoginActivityViewModel loginActivityViewModel;
    private MqttService mqttService;
    private Button loginButton;
    private TextView userIdTextview;
    private TextView passwordTextview;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);


        setContentView(R.layout.activity_login);
        loginButton =(Button) findViewById(R.id.login_button);
        userIdTextview = (TextView) findViewById(R.id.login_username);
        passwordTextview = (TextView) findViewById(R.id.login_password);

        loginButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //loginActivityViewModel.sendLoginMessage(userIdTextview.getText().toString(), passwordTextview.getText().toString());

                try {
                    mqttService.publicMessage(Variables.loginTopic, loginActivityViewModel.getLoginMqttMessage(userIdTextview.getText().toString(), passwordTextview.getText().toString()));
                } catch (MqttException e) {
                    e.printStackTrace();
                }
            }
        });

        loginActivityViewModel = ViewModelProviders.of(this).get(LoginActivityViewModel.class);
        loginActivityViewModel.init();
        setObservers();
        connectMqtt();
        subscribeMqtt();
        mqttService.getMqttPackage().observe(this, new Observer<MqttPackage>() {
            @Override
            public void onChanged(MqttPackage mqttPackage) {
                loginActivityViewModel.checklogin(mqttPackage);
            }
        });
    }

    private void subscribeMqtt() {
        if(mqttService != null){
            mqttService.subscribeTopic(Variables.loginTopic, 0);
        }
    }

    private void connectMqtt() {
        if(mqttService != null){
            mqttService.connect();
        }
    }

    private void setObservers() {

        loginActivityViewModel.getBinder().observe(this, new Observer<MqttService.MQTTBinder>() {
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
        });
        final Intent myIntent = new Intent(this, MainActivity.class);
        loginActivityViewModel.getLoginStage().observe(this, new Observer<Boolean>() {
            @Override
            public void onChanged(Boolean aBoolean) {
                if(aBoolean){
                    //myIntent.extra
//                    myIntent.putExtra("MyClass", loginActivityViewModel.getUser());
                    startActivity(myIntent);
                }
            }
        });
    }

    @Override
    protected void onStop() {
        super.onStop();
        unbindService(loginActivityViewModel.getServiceConnection());
    }
}