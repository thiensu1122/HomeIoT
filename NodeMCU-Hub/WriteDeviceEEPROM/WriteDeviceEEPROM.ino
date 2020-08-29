#include <EEPROM.h>

//DHT11(0, "DHT11 measure temp and humi"),
//RGBLED(1, "Red green blue led");

union UnsignedIntAsBytes {
	uint16_t uival;
	uint8_t bval[2];
} unsignedIntAsBytes;

void clearEEPROM() {
	for (int i = 0 ; i < EEPROM.length() ; i++) {
		EEPROM.write(i, 0);
	}
}

void setup() {
	uint16_t deviceID = 1;
	uint8_t deviceCode = 1;

	unsignedIntAsBytes.uival = deviceID;
	EEPROM.write(unsignedIntAsBytes.bval[1], 0);
	EEPROM.write(unsignedIntAsBytes.bval[2], 1);
	EEPROM.write(deviceCode, 2);
	
	

	

}

void loop() {
}
