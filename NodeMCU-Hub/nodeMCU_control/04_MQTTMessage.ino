#include <ArduinoJson.h>


#define MAX_DEVICELIST 10 // <--- change to whatever


class MQTTMessage {
private:
	int code;
	String message;
	int directionCode;
	int user_id;
	int hub_id;
	NRF24Message nrf24MessageList[10];
	uint8_t messageListCount;


public:
	void setJsonMQTT(String strPayload) {
		StaticJsonDocument<1024> doc;
		DeserializationError error = deserializeJson(doc, strPayload);
		if (error) {
			Serial.print(F("deserializeJson() failed: "));
			Serial.println(error.c_str());
			return;
		}
		JsonObject jsonPayload = doc.as<JsonObject>();
		code = jsonPayload["code"].as<int>();
		message = jsonPayload["message"].as<String>();
		directionCode = jsonPayload["directionCode"].as<int>();
		user_id = jsonPayload["user_id"].as<int>();
		hub_id = jsonPayload["hub_id"].as<int>();
		JsonArray jsonDeviceList = jsonPayload["deviceList"].as<JsonArray>();
		messageListCount = jsonPayload["deviceList"].size();
		for(int i=0; i<messageListCount; i++) {
			nrf24MessageList[i].setJsonData(jsonDeviceList[i]);
			//nrf24MessageList[i].printData();
		}
	}

	void printData() {
		Serial.print("code: ");
		Serial.print(code);
		Serial.print(", message: ");
		Serial.print(message);
		Serial.print(", directionCode: ");
		Serial.print(directionCode);
		Serial.print(", user_id: ");
		Serial.print(user_id);
		Serial.print(", hub_id: ");
		Serial.print(hub_id);
		Serial.print(", messageListCount: ");
		Serial.println(messageListCount);
		for(int i = 0; i< messageListCount; i++) {
			nrf24MessageList[i].printData();
		}
		Serial.println();
	}

	NRF24Message* getNRF24MessageList() {
		return nrf24MessageList;
	}

	uint8_t getNRF24MessageListCount() {
		return messageListCount;
	}

	int getDirectionCode() {
		return directionCode;
	}
	int getCode() {
		return code;
	}
};
