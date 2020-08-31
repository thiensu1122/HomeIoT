#include "DHT.h"

#define DHTPIN 2 ////// connect to D2 
#define DHTTYPE DHT11

class DHTHome {
private :
	DHT dht = DHT(DHTPIN, DHTTYPE);
public:
	void setupDHT() {
		dht.begin();
	}
	void getTempAndHumi(NRF24Message &nrf24Message) {
		// Reading temperature or humidity takes about 250 milliseconds!
		// Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
		float h = dht.readHumidity();
		// Read temperature as Celsius (the default)
		float t = dht.readTemperature();
		if(isnan(h) || isnan(t)) {
			nrf24Message.setStatus(254);
			nrf24Message.setValue1(0);
			nrf24Message.setValue2(0);
			nrf24Message.clearValue3();
		} else {
			nrf24Message.setStatus(1);
			nrf24Message.setValue1(h);
			nrf24Message.setValue2(t);
			nrf24Message.clearValue3();
		}


	}
};
