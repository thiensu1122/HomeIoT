//////////// handle redefinition
#pragma once
#include "Arduino.h"
#include "DataPackage.h"
#include <ArduinoJson.h>

union FloatAsBytes {
	float fval;
	uint8_t bval[4];
} floatAsBytes;

union UnsignedIntAsBytes {
	uint16_t uival;
	uint8_t bval[2];
} unsignedIntAsBytes;

class NRF24Message {
private:
	uint16_t device_id; //////////// to do change to unsigned int 2 bytes
	uint8_t device_code;
	uint8_t status;
	float value1;
	float value2;
	uint8_t value3[20]; /////// 32 - 2(deviceID0 - 1(devicecode) - 1(status) - 4(value1) - 4(value2) = 20
public:
	NRF24Message() {
	}
	NRF24Message(uint16_t device_id) {
		this->device_id = device_id;
		device_code = 0;
		status = -1;
		value1 = 0;
		value2 = 0;
		memset(value3,0,sizeof(value3));
	}

	void setDataPackage(DataPackage dataPackage) {
		device_id = getUnsignedIntFromPackageData(dataPackage.device_id_0,dataPackage.device_id_1 );
		device_code = dataPackage.device_code;
		status = dataPackage._status;
		value1 = getFloatFromPackageData(dataPackage.value1_1,dataPackage.value1_2 ,dataPackage.value1_3, dataPackage.value1_4);
		value2 = getFloatFromPackageData(dataPackage.value2_1,dataPackage.value2_2 ,dataPackage.value2_3, dataPackage.value2_4);
		setValue3FromPackageData(dataPackage.value3_1, dataPackage.value3_2, dataPackage.value3_3, dataPackage.value3_4, dataPackage.value3_5,
		                         dataPackage.value3_6, dataPackage.value3_7, dataPackage.value3_8, dataPackage.value3_9, dataPackage.value3_10,
		                         dataPackage.value3_11, dataPackage.value3_12, dataPackage.value3_13, dataPackage.value3_14, dataPackage.value3_15,
		                         dataPackage.value3_16, dataPackage.value3_17, dataPackage.value3_18, dataPackage.value3_19, dataPackage.value3_20);
	}

	void setJsonData(JsonObject jsonData) {
		device_id = jsonData["device_id"].as<uint16_t>();
		device_code = jsonData["code"].as<uint8_t>();
		status = jsonData["status"].as<uint8_t>();
		value1 = jsonData["value1"].as<int>();
		value2 = jsonData["value2"].as<int>();
		jsonData["value3"].as<String>().getBytes(value3, sizeof(value3));
	}

	void updateValues(NRF24Message nrf24Message) {
		device_code = nrf24Message.device_code;
		status = nrf24Message.status;
		value1 = nrf24Message.value1;
		value2 = nrf24Message.value2;
		memcpy(value3, nrf24Message.value3, sizeof(value3));
	}

	void getMessageBytes(uint8_t *message, uint8_t length) {
		unsignedIntAsBytes.uival = device_id;
		message[0] = unsignedIntAsBytes.bval[0];
		message[1] = unsignedIntAsBytes.bval[1];
		message[2] = device_code;
		message[3] = status;
		floatAsBytes.fval = value1;
		message[4] = floatAsBytes.bval[0];
		message[5] = floatAsBytes.bval[1];
		message[6] = floatAsBytes.bval[2];
		message[7] = floatAsBytes.bval[3];
		floatAsBytes.fval = value2;
		message[8] = floatAsBytes.bval[0];
		message[9] = floatAsBytes.bval[1];
		message[10] = floatAsBytes.bval[2];
		message[11] = floatAsBytes.bval[3];

		for(int i = 0; i< sizeof(value3); i++) {
			message[12+i] = value3[i];
		}
	}
	void printData() {
		Serial.print("id: ");
		Serial.print(device_id);
		Serial.print(", device_code: ");
		Serial.print(device_code);
		Serial.print(", status: ");
		Serial.print(status);
		Serial.print(", value1: ");
		Serial.print(value1);
		Serial.print(", value2: ");
		Serial.print(value2);
		Serial.print(", value3: ");
		for(int i = 0; i< sizeof(value3); i++) {
			Serial.print(value3[i]);
			Serial.print("|");
		}
		Serial.println("");
	}

	void debugData() {
		status = 0;
		value1 = 1.1;
		value2 = -2.2;

		for (int i =0 ; i<sizeof(value3); i++) {
			value3[i] = i;
		}
	}

	unsigned int getUnsignedIntFromPackageData(uint8_t byte1,uint8_t byte2) {
		unsignedIntAsBytes.bval[0] = byte1;
		unsignedIntAsBytes.bval[1] = byte2;
		return unsignedIntAsBytes.uival;
	}

	float getFloatFromPackageData(uint8_t byte1,uint8_t byte2,uint8_t byte3, uint8_t byte4) {

		floatAsBytes.bval[0] = byte1;
		floatAsBytes.bval[1] = byte2;
		floatAsBytes.bval[2] = byte3;
		floatAsBytes.bval[3] = byte4;
		return floatAsBytes.fval;
	}
	void setValue3FromPackageData(uint8_t byte1,uint8_t byte2,uint8_t byte3, uint8_t byte4, uint8_t byte5, uint8_t byte6, uint8_t byte7, uint8_t byte8, uint8_t byte9,uint8_t byte10,
	                              uint8_t byte11,uint8_t byte12,uint8_t byte13, uint8_t byte14, uint8_t byte15, uint8_t byte16, uint8_t byte17, uint8_t byte18, uint8_t byte19,uint8_t byte20) {

		value3[0] = byte1;
		value3[1] = byte2;
		value3[2] = byte3;
		value3[3] = byte4;
		value3[4] = byte5;
		value3[5] = byte6;
		value3[6] = byte7;
		value3[7] = byte8;
		value3[8] = byte9;
		value3[9] = byte10;
		value3[10] = byte11;
		value3[11] = byte12;
		value3[12] = byte13;
		value3[13] = byte14;
		value3[14] = byte15;
		value3[15] = byte16;
		value3[16] = byte17;
		value3[17] = byte18;
		value3[18] = byte19;
		value3[19] = byte20;

	}
	uint16_t getDeviceID() {
		return device_id;
	}
	uint8_t getDeviceCode() {
		return device_code;
	}
	uint8_t getStatus() {
		return status;
	}
	float getValue1() {
		return value1;
	}
	float getValue2() {
		return value2;
	}
	byte *getValue3() {
		return value3;
	}
	String getValue3String() {
		String result = "";
		for(int i =0; i< sizeof(value3); i++) {
			result += value3[i];
		}
		return result;
	}
	void setDeviceCode(uint8_t device_code) {
		this->device_code = device_code;
	}
	void setStatus(uint8_t status) {
		this->status = status;
	}
	void setValue1(float value1) {
		this->value1 = value1;
	}
	void setValue2(float value2) {
		this->value2 = value2;
	}
	void setValue3(byte *value3) {
		memcpy(this->value3, value3, sizeof(value3));
	}
	void clearValue3() {
		memset(value3,0,sizeof(value3));
	}
	void setValue3(byte *newValue3,int length) {
		clearValue3();
		for(int i =0; i< length; i++) {
			value3[i] = newValue3[i];
		}
	}
};
