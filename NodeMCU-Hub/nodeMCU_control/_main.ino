
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include "DataPackage.h"



const char* ssid = "Bao2G";
const char* password = "bao0123456";

NRF24 nrf24 = NRF24();
MQTT mqtt = MQTT();

int gatewayID = EEPROM.read(0);

void setup()
{
	Serial.begin(115200);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(100);
		Serial.print(".");
	}
	Serial.println("");
	Serial.println("WiFi connected");
	mqtt.connectMQTT();
	nrf24.setupNRF();
}

void loop()
{
	mqtt.mqttLoop();
	if(nrf24.NRFLoop()){
		//got message from devices or sensors
			
	}

}
