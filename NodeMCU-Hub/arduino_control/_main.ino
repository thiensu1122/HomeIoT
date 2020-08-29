#include <EEPROM.h>

//MISO connects to pin 12 of the Arduino Nano
//MOSI connects to pin 11 of the Arduino Nano
//SCK connects to pin 13 of the Arduino Nano
//CE connects to pin 7 of the Arduino Nano
//CSN connects to pin 8 of the Arduino Nano


//RH_NRF24 nrf24(2, 4); // use this for NodeMCU Amica/AdaFruit Huzzah ESP8266 Feather
//RH_NRF24 nrf24(7, 8); // use this with Arduino UNO/Nano
NRF24 nrf24 = NRF24();
//int deviceID = EEPROM.read(0);
uint16_t deviceID = 1;
long lastUpdateInfo = 0;
NRF24Message nrf24Message(1);
DHTHome dht11;

void readEEPROM() {
	
}

void setup()
{
	nrf24Message.setDeviceCode(1);
	Serial.begin(115200);
	nrf24.setupNRF();
	dht11.setupDHT();
}


void loop()
{

	long timepassed = millis() - lastUpdateInfo;
	if(timepassed  >= 3000) {

		lastUpdateInfo = millis();
		dht11.getTempAndHumi(nrf24Message);
		nrf24Message.printData();
		nrf24.sendMessage(nrf24Message);

	}
	if(nrf24.NRFLoop()) {
		if(nrf24.getNRF24Message().getDeviceID() == deviceID) {
			//nrf24.getNRF24Message().printData();
		}
	}

}
