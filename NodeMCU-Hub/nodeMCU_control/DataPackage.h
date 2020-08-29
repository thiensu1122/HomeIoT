// Max size of this struct is 32 bytes - NRF24L01 buffer limit
#include "Arduino.h"
//////////// handle redefinition 
#pragma once
struct DataPackage {
  uint8_t device_id_0;
  uint8_t device_id_1;
  uint8_t device_code;
  uint8_t _status;
  uint8_t value1_1;
  uint8_t value1_2;
  uint8_t value1_3;
  uint8_t value1_4;
  uint8_t value2_1;
  uint8_t value2_2;
  uint8_t value2_3;
  uint8_t value2_4;
  uint8_t value3_1;
  uint8_t value3_2;
  uint8_t value3_3;
  uint8_t value3_4;
  uint8_t value3_5;
  uint8_t value3_6;
  uint8_t value3_7;
  uint8_t value3_8;
};


//struct Data_Package {
//  byte j1PotX;
//  byte j1PotY;
//  byte j2PotX;
//  byte j2PotY;
//  byte pot1;
//  byte pot2;
//  byte buttonsSwitches;// including j1,j2,button1,button2,button3,button4,tSwitch1,tSwitch2
//};
//Data_Package data; //Create a variable with the above structure
