
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "DataPackage.h"

#define devicesCount 5
#define deviceID_1 1
#define deviceID_2 2
#define deviceID_3 3
#define deviceID_4 4
#define deviceID_5 5

const char* ssid = "Bao2G";
const char* password = "bao0123456";
NRF24Message nrf24MessageList[5];
NRF24Message nrf24Message;
MQTTMessage mqttmessage;

NRF24 nrf24 = NRF24();
MQTT mqtt = MQTT(); 
int gatewayID = EEPROM.read(0);

StaticJsonDocument<1024> jsonMQTT;

void setup()
{
	Serial.begin(115200);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(100);
		Serial.print(".");
	}
	setupMessageList();
	Serial.println("WiFi connected");
	mqtt.connectMQTT();
	nrf24.setupNRF();
}

void setupMessageList() {
	nrf24MessageList[0] = NRF24Message(deviceID_1);
	nrf24MessageList[1] = NRF24Message(deviceID_2);
	nrf24MessageList[2] = NRF24Message(deviceID_3);
	nrf24MessageList[3] = NRF24Message(deviceID_4);
	nrf24MessageList[4] = NRF24Message(deviceID_5);
}

void updateMessageList() {
	for (int i = 0; i< devicesCount ; i++) {
		if(nrf24MessageList[i].getDeviceID() == nrf24Message.getDeviceID()) {
			nrf24MessageList[i].updateValues(nrf24Message);
		}
	}
}

void clearMessageListStatus() {
	for (int i = 0; i< devicesCount ; i++) {
		nrf24MessageList[i].setStatus(-1);
	}
}

static void mainMQTTCallback(char* topic, byte* payload, unsigned int length){
	Serial.print("Message arrived in topic: ");
	String strPayload = "";
	String strTopic= "";
	for (int i = 0; i < strlen(topic); i++) {
		strTopic += topic[i];
	}
	Serial.println(strTopic);

	
	for (int i = 0; i < length; i++) {
		//Serial.print((char)payload[i]);
		strPayload += (char)payload[i];
	}
	mqttmessage = MQTTMessage();
	mqttmessage.setJsonMQTT(strPayload);
	
	// Deserialize the JSON document
  	
  	

  	
}

void loop()
{
	mqtt.mqttLoop();
	if(mqtt.sendUpdateInfo(nrf24MessageList, devicesCount)) {
		clearMessageListStatus();
	}
	if(nrf24.NRFLoop()) {
		//got message from devices or sensors
		nrf24Message = nrf24.getNRF24Message();
		updateMessageList();
	}
}
