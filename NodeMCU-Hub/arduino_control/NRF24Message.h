//////////// handle redefinition 
#pragma once
#include "Arduino.h"
#include "DataPackage.h"
class NRF24Message {
  public:
  NRF24Message(DataPackage dataPackage);
  NRF24Message(uint8_t device_id);
  uint8_t getDeviceID();
  uint8_t getCode();
  uint8_t getStatus();
  float getValue1();
  float getValue2();
  String getValue3();
  void setCode(uint8_t code);
  void setStatus(uint8_t status);
  void setValue1(float value1);
  void setValue2(float value2);
  void setValue3(String value3);
  void getMessageBytes(uint8_t *message, uint8_t length);
  void printData();
  
  float getFloatFromPackageData(uint8_t byte1,uint8_t byte2,uint8_t byte3, uint8_t byte4);
  String getStringFromPackageData(uint8_t byte1,uint8_t byte2,uint8_t byte3, uint8_t byte4, uint8_t byte5, uint8_t byte6, uint8_t byte7, uint8_t byte8);
  private:
    uint8_t device_id;
    uint8_t code;
    uint8_t status;
    float value1;
    float value2;
    String value3;
};
