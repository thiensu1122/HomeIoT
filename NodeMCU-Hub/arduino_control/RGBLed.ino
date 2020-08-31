/*
 	Wiring pin:
 	D3 - 3 -> RED
 	D4 - 5 -> Green
 	D5 - 6 -> Blue
*/
class RGBLED {
private:
	uint8_t red_light_pin= 3;
	uint8_t green_light_pin = 5;
	uint8_t blue_light_pin = 6;
	uint8_t red_light_value;
	uint8_t green_light_value;
	uint8_t blue_light_value;
public:
	void setup() {
		pinMode(red_light_pin, OUTPUT);
		pinMode(green_light_pin, OUTPUT);
		pinMode(blue_light_pin, OUTPUT);
	}

	void RGB_color(uint8_t red_light_value, uint8_t green_light_value, uint8_t blue_light_value)
	{
		this->red_light_value = red_light_value;
		this->green_light_value = green_light_value;
		this->blue_light_value = blue_light_value;
		analogWrite(red_light_pin, red_light_value);
		analogWrite(green_light_pin, green_light_value);
		analogWrite(blue_light_pin, blue_light_value);
	}
	void getColors(NRF24Message &nrf24Message) {
		byte newValue3[3];
		nrf24Message.setStatus(1);
		newValue3[0]=red_light_value;
		newValue3[1]=green_light_value;
		newValue3[2]=blue_light_value;
		nrf24Message.setValue3(newValue3, 3);
	}
	
};
