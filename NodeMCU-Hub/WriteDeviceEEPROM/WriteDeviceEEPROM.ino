#include <EEPROM.h>


union UnsignedIntAsBytes {
	uint16_t uival;
	uint8_t bval[2];
} unsignedIntAsBytes;

void setup() {
	uint16_t deviceID = 1;
	uint8_t deviceCode = 1;
	
	
	for (int i = 0 ; i < EEPROM.length() ; i++) {
		EEPROM.write(i, 0);
	}
	for (int i =0 ; i< 3; i++) {
		EEPROM.write(i, 0);
	}
}

void loop() {
}
