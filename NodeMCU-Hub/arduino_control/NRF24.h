#include <RH_NRF24.h>

#include "NRF24Message.h"

//MISO connects to pin 12 of the Arduino Nano
//MOSI connects to pin 11 of the Arduino Nano
//SCK connects to pin 13 of the Arduino Nano
//CE connects to pin 7 of the Arduino Nano
//CSN connects to pin 8 of the Arduino Nano

const byte address[6] = "00001";

void setupNRF(RH_NRF24 nrf24);
void sendMessage(RH_NRF24 nrf24, NRF24Message data);
