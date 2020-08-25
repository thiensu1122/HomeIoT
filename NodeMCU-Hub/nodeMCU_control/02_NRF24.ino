#include <RH_NRF24.h>

class NRF24 {
private:
	//MISO connects to pin D6 of the NodeMCU
	//MOSI connects to pin D7 of the NodeMCU
	//SCK connects to pin D5 of the NodeMCU
	//CE connects to pin D4 of the NodeMCU
	//CSN connects to pin D2 of the NodeMCU
	RH_NRF24 _nrf24 = RH_NRF24(2, 4); // use this for NodeMCU Amica/AdaFruit Huzzah ESP8266 Feather
	NRF24Message nrf24Message;
public:
	void setupNRF() {
		if (!_nrf24.init())
		{
			Serial.println("init failed");
		}
		// Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
		if (!_nrf24.setChannel(3))
		{
			Serial.println("setChannel failed");
		}
		if (!_nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm)) {
			Serial.println("setRF failed");
		}

	}

	boolean NRFLoop() {
		if (_nrf24.available())
		{
			// Should be a message for us now
			uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
			uint8_t len = sizeof(DataPackage);
			memset(buf,0,sizeof(buf));
			if (_nrf24.recv(buf, &len))
			{
				// Send a reply
				uint8_t sdata[] = "Data Received.";
				_nrf24.send(sdata, sizeof(sdata));
				_nrf24.waitPacketSent();
				DataPackage dataPackage;
				memcpy( &dataPackage, buf, sizeof( DataPackage ) );
				nrf24Message.setDataPackage(dataPackage);
//				nrf24Message.printData();
			}
			return true;
		} else {
			return false;
		}
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
		_nrf24.send(message, sizeof(DataPackage));
		_nrf24.waitPacketSent();

		// Now wait for a reply

		uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
		uint8_t len = sizeof(buf);

		if (_nrf24.waitAvailableTimeout(2000))
		{
			// Should be a reply message for us now
			if (_nrf24.recv(buf, &len))
			{
				Serial.print("got reply: ");
				Serial.println((char*)buf);
			}
			else
			{
				Serial.println("recv failed");
			}
		}
		else
		{
			Serial.println("No reply.");
		}
		delay(3000);
	}
	NRF24Message getNRF24Message(){
		return nrf24Message;
	}
	

};
