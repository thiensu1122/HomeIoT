#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <ArduinoJson.h>


#define UPDATEINFOINTERVAL  5000
#define mqttBuffer 1024

#define MQTTHUB "hub"
#define MQTTSERVER "server"
#define MQTTDEVICE "device"

#define UPDATEALLDEVICESSTATUS 100
#define ALLDEVICESMESSAGE "ALL DEVICES"
#define HUBTOSERVER "0"
#define TEMPSENSOR "52"
#define CO2SENSOR "53"

#define SERVERTOHUB 3
#define ANDROIDTOHUB 5

#define MESSAGECODE_CONTROLDEVICE 200


class MQTT {
private:
	// ------- mqtt ----- ///
	const char* mqttServer = "192.168.1.15";
	const int mqttPort = 1883;
	const char* mqttUser = "";
	const char* mqttPassword = "";

	String HomeIotTopic = "Smarthome-1";
	WiFiClient espClient;
	//init MQTT
	PubSubClient mqttClient = PubSubClient(espClient);;
	unsigned long lastStatusSend = 0;
	unsigned long lastUpdateInfo = 0;
public:
	void connectMQTT() {
		//init wifi client

		mqttClient.setServer(mqttServer, mqttPort);
		mqttClient.setCallback(&callback);
		mqttClient.setBufferSize(mqttBuffer);

		while (!mqttClient.connected()) {
			Serial.println("Connecting to MQTT...");

			if (mqttClient.connect("ESP8266Client", mqttUser, mqttPassword )) {
				Serial.println("connected");
			} else {
				Serial.print("failed with state ");
				Serial.print(mqttClient.state());
				delay(2000);
			}
		}
		mqttClient.subscribe(HomeIotTopic.c_str());

		//[todo] make led blink for mqtt status.....
	}

	void checkMQTTConnect() {
		if (!mqttClient.connected()) {
			connectMQTT();
		}
	}

	void publicMessage(String data) {
		if(mqttClient.connected()) {
			mqttClient.publish(HomeIotTopic.c_str(),data.c_str());
//			Serial.print("data :" );
//			Serial.println(data.c_str());

		}
	}

	String createMessage(NRF24Message *nrf24MessageList, uint8_t listCount) {
		String message;
		// initial 200 bytes for json document.
		StaticJsonDocument<mqttBuffer> jsonDoc;
		jsonDoc["code"] = UPDATEALLDEVICESSTATUS;
		jsonDoc["message"] = ALLDEVICESMESSAGE;
		jsonDoc["direcionCode"] = "0";
		JsonArray arraySensorData = jsonDoc.createNestedArray("deviceList");;
		for (int i = 0; i< listCount; i++) {
			StaticJsonDocument<200> jsonSensor;
			jsonSensor["device_id"] = nrf24MessageList[i].getDeviceID();
			jsonSensor["device_code"] = nrf24MessageList[i].getDeviceCode();
			jsonSensor["status"] = nrf24MessageList[i].getStatus();
			jsonSensor["value1"] = nrf24MessageList[i].getValue1();
			jsonSensor["value2"] = nrf24MessageList[i].getValue2();
			jsonSensor["value3"] = nrf24MessageList[i].getValue3();
			arraySensorData.add(jsonSensor);
		}
		serializeJsonPretty(jsonDoc, message);
		return message;
	}

	static void callback(char* topic, byte* payload, unsigned int length) {
		mainMQTTCallback(topic, payload, length);
	}

	boolean sendUpdateInfo(NRF24Message *nrf24MessageList, uint8_t listCount) {
		long timepassed = millis() - lastUpdateInfo;
		if(timepassed  >= UPDATEINFOINTERVAL) {
			lastUpdateInfo = millis();
			String message = createMessage(nrf24MessageList, listCount);
			//Serial.println(message);
			publicMessage(message);
			return true;
		}
		return false;
	}

	void mqttLoop() {
		mqttClient.loop();
		checkMQTTConnect();
	}
};
