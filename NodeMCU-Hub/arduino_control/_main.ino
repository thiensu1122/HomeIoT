#include <EEPROM.h>

/*
 * freepin :
 * D2	-> 2
 * D3~	-> 3
 * D4	-> 4
 * D5~	-> 5
 * D6~	-> 6
 * D9~	-> 9
 * D10~ -> 10
 * ~ : PWM
 * A0 --> A7
	DHT11 --> pin D2
	GRB led -> PIN D3,D5,D6

*/

#define RGBLEDDeviceCode 1

NRF24 nrf24 = NRF24();
uint16_t device_id = 1;
uint8_t device_code = 1;
long lastUpdateInfo = 0;
NRF24Message nrf24Message;
DHTHome dht11;
RGBLED rgbLED;
void readEEPROM() {
	unsignedIntAsBytes.bval[0] = EEPROM.read(0);
	unsignedIntAsBytes.bval[1] = EEPROM.read(1);
	device_id = unsignedIntAsBytes.uival;
	device_code = EEPROM.read(2);
//	Serial.println(device_id);
//	Serial.println(device_code);
}

void setup()
{
	Serial.begin(115200);

	//readEEPROM();
	nrf24Message = NRF24Message(device_id);
	nrf24Message.setDeviceCode(device_code);
	nrf24.setupNRF();
	if(device_code == 0) dht11.setupDHT();
	if(device_code == 1) rgbLED.setup();
}


void loop()
{

	long timepassed = millis() - lastUpdateInfo;
	if(timepassed  >= 3000) {

		lastUpdateInfo = millis();
		
		if(device_code == 0) dht11.getTempAndHumi(nrf24Message);
		if(device_code == 1) rgbLED.getColors(nrf24Message);
		nrf24Message.debugData();
		nrf24Message.printData();
		nrf24.sendMessage(nrf24Message);

	}
	if(nrf24.NRFLoop()) {
		if(nrf24.getNRF24Message().getDeviceID() == device_id && nrf24.getNRF24Message().getDeviceCode() == device_code) {
			//nrf24.getNRF24Message().printData();
			if (nrf24.getNRF24Message().getDeviceCode() == RGBLEDDeviceCode){
				floatAsBytes.fval = nrf24.getNRF24Message().getValue1();
				rgbLED.RGB_color(floatAsBytes.bval[0],floatAsBytes.bval[1],floatAsBytes.bval[2]);
			}
			
		}
	}

}
