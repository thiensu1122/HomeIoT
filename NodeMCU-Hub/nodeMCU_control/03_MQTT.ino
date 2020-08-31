#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <ArduinoJson.h>


#define NRF24DEBUG false

#define UPDATEINFOINTERVAL  5000
#define mqttBuffer 1024

#define MQTTHUB "hub"
#define MQTTSERVER "server"
#define MQTTDEVICE "device"

#define UPDATEALLDEVICESSTATUS 100
#define UPDATESINGLEDEVICESTATUS 101
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

//	String createMessage(NRF24Message *nrf24MessageList, uint8_t listCount) {
//		String message;
//		// initial 200 bytes for json document.
//		StaticJsonDocument<mqttBuffer> jsonDoc;
//		jsonDoc["code"] = UPDATEALLDEVICESSTATUS;
//		jsonDoc["message"] = ALLDEVICESMESSAGE;
//		jsonDoc["direcionCode"] = "0";
//		JsonArray arraySensorData = jsonDoc.createNestedArray("deviceList");
//		for (int i = 0; i< listCount; i++) {
//			StaticJsonDocument<500> jsonSensor;
//			jsonSensor["device_id"] = nrf24MessageList[i].getDeviceID();
//			jsonSensor["device_code"] = nrf24MessageList[i].getDeviceCode();
//			jsonSensor["status"] = nrf24MessageList[i].getStatus();
//			jsonSensor["value1"] = nrf24MessageList[i].getValue1();
//			jsonSensor["value2"] = nrf24MessageList[i].getValue2();
//			//jsonSensor["value3"] = nrf24MessageList[i].getValue3String();
//			JsonArray value3bytes = jsonSensor.createNestedArray("value3");
//			for(int j=0; j<20; j++) {
//				value3bytes.add(j);
//			}
//
//			arraySensorData.add(jsonSensor);
//			Serial.print(" measureJson jsonSensor:");
//			Serial.print(measureJson(jsonSensor));
//		}
//
//		serializeJson(jsonDoc, message);
//		Serial.print(" measureJson :");
//		Serial.println(measureJson(jsonDoc));
//
//		Serial.println(message);
//		return message;
//	}

	String createMessage(NRF24Message nrf24Message) {
		String message;
		StaticJsonDocument<mqttBuffer> jsonDoc;
		jsonDoc["code"] = UPDATESINGLEDEVICESTATUS;
		jsonDoc["message"] = ALLDEVICESMESSAGE;
		jsonDoc["direcionCode"] = 0;
		JsonArray arraySensorData = jsonDoc.createNestedArray("deviceList");

		StaticJsonDocument<500> jsonSensor;
		jsonSensor["device_id"] = nrf24Message.getDeviceID();
		jsonSensor["device_code"] = nrf24Message.getDeviceCode();
		jsonSensor["status"] = nrf24Message.getStatus();
		jsonSensor["value1"] = nrf24Message.getValue1();
		jsonSensor["value2"] = nrf24Message.getValue2();
		//jsonSensor["value3"] = nrf24MessageList[i].getValue3String();
		JsonArray value3bytes = jsonSensor.createNestedArray("value3");
		for(int j=0; j<nrf24Message.getValue3Size(); j++) {
			value3bytes.add(nrf24Message.getValue3()[j]);
		}

		arraySensorData.add(jsonSensor);
		serializeJson(jsonDoc, message);
		if(NRF24DEBUG) {
			Serial.print(" measureJson jsonSensor:");
			Serial.print(measureJson(jsonSensor));
			Serial.print(" measureJson :");
			Serial.println(measureJson(jsonDoc));
			Serial.println(message);
		}
		return message;
	}

	static void callback(char* topic, byte* payload, unsigned int length) {
		mainMQTTCallback(topic, payload, length);
	}

	boolean sendUpdateInfo(NRF24Message *nrf24MessageList, uint8_t listCount) {
		long timepassed = millis() - lastUpdateInfo;
		if(timepassed  >= UPDATEINFOINTERVAL) {
			lastUpdateInfo = millis();
			String message = "";
			for(int i=0; i< listCount; i++) {
				message = createMessage(nrf24MessageList[i]);
				//Serial.println(message);
				publicMessage(message);
				delay(10);
			}
			return true;
		}
		return false;
	}

	void mqttLoop() {
		mqttClient.loop();
		checkMQTTConnect();
	}
};
