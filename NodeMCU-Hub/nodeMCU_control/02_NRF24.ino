#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define ARDUINOTONODEMCUCHANNEL 1
#define NODEMCUTOARDUINOCHANNEL 2

class NRF24 {
private:
	//MISO connects to pin D6 of the NodeMCU
	//MOSI connects to pin D7 of the NodeMCU
	//SCK connects to pin D5 of the NodeMCU
	//CE connects to pin D4 of the NodeMCU
	//CSN connects to pin D2 of the NodeMCU


#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)

	RF24 radio = RF24(7, 8); // use this for NodeMCU Amica/AdaFruit Huzzah ESP8266 Feather
	//Code in here will only be compiled if an Arduino Uno (or older) is used.
#else

	RF24 radio = RF24(2, 4); // use this for NodeMCU Amica/AdaFruit Huzzah ESP8266 Feather

#endif
	const byte addresses[2][6] = {"00001", "00002"};
	NRF24Message nrf24Message;
public:

	void setupNRF() {

		radio.begin();

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
		radio.openWritingPipe(addresses[1]); // 00002
		radio.openReadingPipe(1, addresses[0]); // 00001
#else
		radio.openWritingPipe(addresses[0]); // 00001
		radio.openReadingPipe(1, addresses[1]); // 00002
#endif
		radio.setPALevel(RF24_PA_MIN);
	}

	boolean NRFLoop() {
		radio.startListening();
		if (radio.available())
		{
			// Should be a message for us now
			uint8_t buf[32];
			uint8_t len = sizeof(DataPackage);
			memset(buf,0,sizeof(buf));
			radio.read(buf, len);

			// Send a reply
			DataPackage dataPackage;
			memcpy( &dataPackage, buf, sizeof( DataPackage ) );
			nrf24Message.setDataPackage(dataPackage);
			nrf24Message.printData();

			return true;
		} else {
			return false;
		}

//		delay(5);
//		radio.startListening();
//		if ( radio.available()) {
//			//Serial.println("Something");
//			while (radio.available()) {
//				int angleV = 0;
//				radio.read(&angleV, sizeof(angleV));
//				Serial.println("Something");
//			}
//			delay(5);
//			radio.stopListening();
//
//		}
	}
	void printMessage(byte *message, uint8_t length) {
		for(int i = 0; i< length; i++) {
			Serial.print(message[i]);
			Serial.print("|");
		}
		Serial.println();
	}

	void sendMessage(NRF24Message nrf24Message) {

		nrf24Message.printData();
		uint8_t message[sizeof(DataPackage)];
		memset(message,0,sizeof(message));
		nrf24Message.getMessageBytes(message,sizeof(DataPackage));

		delay(5);
		radio.stopListening();
		radio.write(&message, sizeof(DataPackage));
		delay(5);
		radio.startListening();





	}
	NRF24Message getNRF24Message() {
		return nrf24Message;
	}


};
