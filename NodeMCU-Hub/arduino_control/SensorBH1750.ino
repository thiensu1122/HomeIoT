#include <Wire.h>
#include <BH1750.h>

//VCC -> 3V3 or 5V
//GND -> GND
//SCL -> SCL (A5 on Arduino Nano, Uno, Leonardo, etc or 21 on Mega and Due, on esp8266 free selectable)
//SDA -> SDA (A4 on Arduino Nano, Uno, Leonardo, etc or 20 on Mega and Due, on esp8266 free selectable)
//ADD -> NC/GND or VCC
class SensorBH1750 {
private:

	BH1750 lightMeter;

public:
	void setup() {
		// Initialize the I2C bus (BH1750 library doesn't do this automatically)
		Wire.begin();
		// On esp8266 you can select SCL and SDA pins using Wire.begin(D4, D3);
		lightMeter.begin();
	}
	void getLux(NRF24Message &nrf24Message) {
		nrf24Message.setStatus(1);
		nrf24Message.setValue1(lightMeter.readLightLevel());
	}
};
