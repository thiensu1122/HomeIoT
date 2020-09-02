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
		QM135 -> A0
		BH1750 --> PIN A4 A5 I2C
		RELAY -> D4
*/

#define INTERVAL_UPDATE 3000
#define INTERVAL_EMEGENCY 1000

#define DeviceCode_DHT11 0
#define DeviceCode_RGBLED 1
#define DeviceCode_MQ135 2
#define DeviceCode_BH1750 3
#define DeviceCode_RELAY 4

NRF24 nrf24 = NRF24();
uint16_t device_id = 1;
uint8_t device_code = DeviceCode_MQ135;
long lastUpdateInfo = 0;
NRF24Message nrf24Message;
DHTHome dht11;
RGBLED rgbLED;
SensorMQ135 sensorMQ135;
SensorBH1750 bh1750;
Relay relay;
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
	if(device_code == DeviceCode_DHT11) dht11.setupDHT();
	if(device_code == DeviceCode_RGBLED) rgbLED.setup();
	if(device_code == DeviceCode_MQ135) sensorMQ135.setup();
	if(device_code == DeviceCode_BH1750) bh1750.setup();
	if(device_code == DeviceCode_RELAY) relay.setup();
}


void loop()
{

	long timepassed = millis() - lastUpdateInfo;
	if(timepassed  >= 3000) {
		lastUpdateInfo = millis();
		if(device_code == DeviceCode_DHT11) dht11.getTempAndHumi(nrf24Message);
		if(device_code == DeviceCode_RGBLED) rgbLED.getColors(nrf24Message);
		if(device_code == DeviceCode_BH1750) bh1750.getLux(nrf24Message);
		if(device_code == DeviceCode_RELAY) relay.getValue(nrf24Message);
		//nrf24Message.debugData();
		//nrf24Message.printData();
		nrf24.sendMessage(nrf24Message);

	}
	//incase of MQ135 we have emergency. therefore, we sample every 500ms.
	if(device_code == DeviceCode_MQ135){
		//return true mean it is emegency else just update nrf24Message as normal
		if(sensorMQ135.loopMQ135(INTERVAL_EMEGENCY, nrf24Message)){
			nrf24.sendMessageEmegency(nrf24Message);
		}
	}
	if(nrf24.NRFLoop()) {
		////// when there are nrf24 message from Nodemcu.
		if(nrf24.getNRF24Message().getDeviceID() == device_id && nrf24.getNRF24Message().getDeviceCode() == device_code) {
			//nrf24.getNRF24Message().printData();
			if (nrf24.getNRF24Message().getDeviceCode() == DeviceCode_RGBLED){
				rgbLED.setColors(nrf24.getNRF24Message());
				rgbLED.getColors(nrf24Message);
				nrf24.sendMessageConfirm(nrf24Message);
			}
			if (nrf24.getNRF24Message().getDeviceCode() == DeviceCode_RELAY){
				relay.setSwitch(nrf24.getNRF24Message());
				relay.getValue(nrf24Message);
				nrf24.sendMessageConfirm(nrf24Message);
			}
			
		}
	}

}
