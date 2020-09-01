
class Relay {
private:
	int Relay = 4;
public:
	void setup() {
		pinMode(Relay, OUTPUT);       // declare Relay as output
	}
	void turnOn() {
		digitalWrite (Relay, LOW);
	}
	void turnOff() {
		digitalWrite (Relay, HIGH);
	}
	void getValue(NRF24Message &nrf24Message) {
		int state = digitalRead(Relay);
		nrf24Message.setStatus(1);
		nrf24Message.setValue1(state);
	}
	void setSwitch(NRF24Message nrf24Message) {
		if(nrf24Message.getValue1() == 1) {
			turnOn();
		} else {
			turnOff();
		}
	}

};
