#include <ArduinoJson.h>


#define MAX_DEVICELIST 10 // <--- change to whatever


class MQTTMessage {
private:
	int code;
	String message;
	String directionCode;
	int user_id;
	int hub_id;
	NRF24Message messageList[10];
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
		directionCode = jsonPayload["directionCode"].as<String>();
		user_id = jsonPayload["user_id"].as<int>();
		hub_id = jsonPayload["hub_id"].as<int>();
		JsonArray jsonDeviceList = jsonPayload["sensorList"].as<JsonArray>();
		messageListCount = jsonPayload["sensorList"].size();
		Serial.print("sizeeeeeeeee :" );
		Serial.println(jsonPayload["sensorList"].size());
		for(int i=0; i<messageListCount; i++) {
			messageList[i].setJsonData(jsonDeviceList[i]);
			messageList[i].printData();
		}

	}
};
