#include <MQUnifiedsensor.h>
//Include the library

//Definitions

#define Voltage_Resolution 5
#define placa "Arduino UNO"
#define pin A0 //Analog input 0 of your arduino
#define type "MQ-135" //MQ135
#define ADC_Bit_Resolution 10 // For arduino UNO/MEGA/NANO
#define RatioMQ135CleanAir 3.6//RS / R0 = 3.6 ppm  
//#define calibration_button 13 //Pin to calibrate your sensor
#define DEBUGMQ135 false
//Declare Sensor

class SensorMQ135 {
private :
	MQUnifiedsensor MQ135;
	unsigned int lastReadTime = 0;
	float CO =0;
	float Alcohol=0;
	float CO2 =0;
	float Tolueno=0;
	float NH4= 0;
	float Acetona=0;
public:
	void setup() {
		MQ135 = MQUnifiedsensor(placa, Voltage_Resolution, ADC_Bit_Resolution, pin, type);
		//Init the serial port communication - to debug the library
		//Set math model to calculate the PPM concentration and the value of constants
		MQ135.setRegressionMethod(1); //_PPM =  a*ratio^b
		/*****************************  MQ Init ********************************************/
		//Remarks: Configure the pin of arduino as input.
		/************************************************************************************/
		MQ135.init();
		/*
		  //If the RL value is different from 10K please assign your RL value with the following method:
		  MQ135.setRL(10);
		*/
		/*****************************  MQ CAlibration ********************************************/
		// Explanation:
		// In this routine the sensor will measure the resistance of the sensor supposing before was pre-heated
		// and now is on clean air (Calibration conditions), and it will setup R0 value.
		// We recomend execute this routine only on setup or on the laboratory and save on the eeprom of your arduino
		// This routine not need to execute to every restart, you can load your R0 if you know the value
		// Acknowledgements: https://jayconsystems.com/blog/understanding-a-gas-sensor
//  Serial.print("Calibrating please wait.");
		float calcR0 = 0;
		for(int i = 1; i<=10; i ++)
		{
			MQ135.update(); // Update data, the arduino will be read the voltage on the analog pin
			calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
//    Serial.print(".");
		}
		MQ135.setR0(calcR0/10);
//  Serial.println("  done!.");

		if(isinf(calcR0)) {
			Serial.println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your wiring and supply");
			while(1);
		}
		if(calcR0 == 0) {
			Serial.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please check your wiring and supply");
			while(1);
		}
		/*****************************  MQ CAlibration ********************************************/
		if(DEBUGMQ135) {
			Serial.println("** Lectures from MQ-135 ****");
			Serial.println("|    CO   |  Alcohol |   CO2  |  Tolueno  |  NH4  |  Acteona  |");
		}
	}

	void readValues() {
		MQ135.update(); // Update data, the arduino will be read the voltage on the analog pin

		MQ135.setA(605.18);
		MQ135.setB(-3.937); // Configurate the ecuation values to get CO concentration
		CO = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

		MQ135.setA(77.255);
		MQ135.setB(-3.18); // Configurate the ecuation values to get Alcohol concentration
		Alcohol = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

		MQ135.setA(110.47);
		MQ135.setB(-2.862); // Configurate the ecuation values to get CO2 concentration
		CO2 = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

		MQ135.setA(44.947);
		MQ135.setB(-3.445); // Configurate the ecuation values to get Tolueno concentration
		Tolueno = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

		MQ135.setA(102.2 );
		MQ135.setB(-2.473); // Configurate the ecuation values to get NH4 concentration
		NH4 = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

		MQ135.setA(34.668);
		MQ135.setB(-3.369); // Configurate the ecuation values to get Acetona concentration
		Acetona = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup
		if(DEBUGMQ135) {
			Serial.print("|   ");
			Serial.print(CO);
			Serial.print("   |   ");
			Serial.print(Alcohol);
			Serial.print("   |   ");
			Serial.print(CO2);
			Serial.print("   |   ");
			Serial.print(Tolueno);
			Serial.print("   |   ");
			Serial.print(NH4);
			Serial.print("   |   ");
			Serial.print(Acetona);
			Serial.println("   |");
		}
		/*
		  Exponential regression:
		GAS      | a      | b
		CO       | 605.18 | -3.937
		Alcohol  | 77.255 | -3.18
		CO2      | 110.47 | -2.862
		Tolueno  | 44.947 | -3.445
		NH4      | 102.2  | -2.473
		Acetona  | 34.668 | -3.369
		*/
	}
	/////// true if there is emergency 
	boolean loopMQ135(uint8_t interval, NRF24Message &nrf24Message) {
		if((millis() - lastReadTime)>interval) {
			nrf24Message.setStatus(1);
			readValues();
			nrf24Message.setValue1(CO);
			nrf24Message.setValue2(Alcohol);
			floatAsBytes.fval= CO2;
			nrf24Message.setValue3(floatAsBytes.bval, 4);
			floatAsBytes.fval= Tolueno;
			nrf24Message.setValue3(floatAsBytes.bval, 4, 7);
			floatAsBytes.fval= NH4;
			nrf24Message.setValue3(floatAsBytes.bval, 8, 11);
			floatAsBytes.fval= Acetona;
			nrf24Message.setValue3(floatAsBytes.bval, 12, 15);
			if(CO2>50) {
				return true;
			}
			return false;
		}
		return false;
	}
};
