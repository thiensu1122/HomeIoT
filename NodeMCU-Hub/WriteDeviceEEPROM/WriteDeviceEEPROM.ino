#include <EEPROM.h>

//DHT11(0, "DHT11 measure temp and humi"),
//RGBLED(1, "Red green blue led");

uint16_t deviceID;
uint8_t deviceCode;

union UnsignedIntAsBytes {
	uint16_t uival;
	uint8_t bval[2];
} unsignedIntAsBytes;

void clearEEPROM() {
	for (int i = 0 ; i < EEPROM.length() ; i++) {
		EEPROM.write(i, 0);
	}
}

void writeEEPROM() {
	clearEEPROM();
	unsignedIntAsBytes.uival = deviceID;
	EEPROM.write(0, unsignedIntAsBytes.bval[0]);
	EEPROM.write(1, unsignedIntAsBytes.bval[1]);
	EEPROM.write(2, deviceCode);
}

void readEEPROM() {
	unsignedIntAsBytes.bval[0] = EEPROM.read(0);
	unsignedIntAsBytes.bval[1] = EEPROM.read(1);
	uint16_t device_id = unsignedIntAsBytes.uival;
	uint8_t device_code = EEPROM.read(2);
	Serial.println(device_id);
	Serial.println(device_code);
}

void setup() {
	Serial.begin(115200);
	deviceID = 1;
	deviceCode = 1;
	writeEEPROM();
	readEEPROM();
}

void loop() {
}
