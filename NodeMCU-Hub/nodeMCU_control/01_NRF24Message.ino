//////////// handle redefinition
#pragma once
#include "Arduino.h"
#include "DataPackage.h"


union FloatAsBytes {
	float fval;
	uint8_t bval[4];
} floatAsBytes;

class NRF24Message {
private:
	uint8_t device_id;
	uint8_t code;
	uint8_t status;
	float value1;
	float value2;
	String value3;
public:
	NRF24Message(){
	}
	NRF24Message(uint8_t device_id) {
		this->device_id = device_id;
		code = 0;
		status = -1;
		value1 = 0;
		value2 = 0;
		value3 = "";
	}
	
	void setDataPackage(DataPackage dataPackage) {
		device_id = dataPackage.device_id;
		code = dataPackage.code;
		status = dataPackage._status;
		value1 = getFloatFromPackageData(dataPackage.value1_1,dataPackage.value1_2 ,dataPackage.value1_3, dataPackage.value1_4);
		value2 = getFloatFromPackageData(dataPackage.value2_1,dataPackage.value2_2 ,dataPackage.value2_3, dataPackage.value2_4);
		value3 = getStringFromPackageData(dataPackage.value3_1,dataPackage.value3_2 ,dataPackage.value3_3, dataPackage.value3_4,dataPackage.value3_5,dataPackage.value3_6 ,dataPackage.value3_7, dataPackage.value3_8);
	}

	void updateValues(NRF24Message nrf24Message) {
		code = nrf24Message.code;
		status = nrf24Message.status;
		value1 = nrf24Message.value1;
		value2 = nrf24Message.value2;
		value3 = nrf24Message.value3;
	}
	
	void getMessageBytes(uint8_t *message, uint8_t length) {

		message[0] = device_id;
		message[1] = code;
		message[2] = status;
		floatAsBytes.fval = value1;
		message[3] = floatAsBytes.bval[0];
		message[4] = floatAsBytes.bval[1];
		message[5] = floatAsBytes.bval[2];
		message[6] = floatAsBytes.bval[3];
		floatAsBytes.fval = value2;
		message[7] = floatAsBytes.bval[0];
		message[8] = floatAsBytes.bval[1];
		message[9] = floatAsBytes.bval[2];
		message[10] = floatAsBytes.bval[3];
		byte bytes[value3.length() + 1];
		value3.getBytes(bytes, value3.length() + 1);
		for(int i = 0; i< value3.length(); i++) {
			message[11+i] = bytes[i];
		}
	}
	void printData() {
		Serial.print("id: ");
		Serial.print(device_id);
		Serial.print(", code: ");
		Serial.print(code);
		Serial.print(", status: ");
		Serial.print(status);
		Serial.print(", value1: ");
		Serial.print(value1);
		Serial.print(", value2: ");
		Serial.print(value2);
		Serial.print(", value3: ");
		Serial.println(value3);
	}


	float getFloatFromPackageData(uint8_t byte1,uint8_t byte2,uint8_t byte3, uint8_t byte4) {

		floatAsBytes.bval[0] = byte1;
		floatAsBytes.bval[1] = byte2;
		floatAsBytes.bval[2] = byte3;
		floatAsBytes.bval[3] = byte4;
		return floatAsBytes.fval;
	}
	String getStringFromPackageData(uint8_t byte1,uint8_t byte2,uint8_t byte3, uint8_t byte4, uint8_t byte5, uint8_t byte6, uint8_t byte7, uint8_t byte8) {
		String stringOne =  "";
		stringOne +=(char) byte1;
		stringOne +=(char) byte2;
		stringOne +=(char) byte3;
		stringOne +=(char) byte4;
		stringOne +=(char) byte5;
		stringOne +=(char) byte6;
		stringOne +=(char) byte7;
		stringOne +=(char) byte8;

		return stringOne;
	}
	uint8_t getDeviceID() {
		return device_id;
	}
	uint8_t getCode() {
		return code;
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
	String getValue3() {
		return value3;
	}
	void setCode(uint8_t code) {
		this->code = code;
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
	void setValue3(String value3) {
		this->value3 = value3;
	}
};
