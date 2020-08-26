#include "DHT.h"
#include <EEPROM.h>


//MISO connects to pin 12 of the Arduino Nano
//MOSI connects to pin 11 of the Arduino Nano
//SCK connects to pin 13 of the Arduino Nano
//CE connects to pin 7 of the Arduino Nano
//CSN connects to pin 8 of the Arduino Nano


//RH_NRF24 nrf24(2, 4); // use this for NodeMCU Amica/AdaFruit Huzzah ESP8266 Feather
//RH_NRF24 nrf24(7, 8); // use this with Arduino UNO/Nano
NRF24 nrf24 = NRF24();
int deviceID = EEPROM.read(0);

void setup()
{
	Serial.begin(115200);
	nrf24.setupNRF();
}


void loop()
{

	Serial.println("Sending to gateway");
	NRF24Message nrf24Message(1);
	nrf24Message.setCode(2);
	nrf24Message.setStatus(3);
	nrf24Message.setValue1(11);
	nrf24Message.setValue2(-22);
	nrf24Message.setValue3("12345");
	nrf24.sendMessage(nrf24Message);
	delay(3000);
}
