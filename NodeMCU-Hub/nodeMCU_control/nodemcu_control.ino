
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include "DataPackage.h"
#include "NRF24Message.h"

#include "MQTT.h"
 

const char* ssid = "Bao2G";
const char* password = "bao0123456";

NRF24 nrf24 = NRF24();
int gatewayID = EEPROM.read(0);
 
void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  connectMQTT();
  nrf24.setupNRF();
}
 
void loop()
{
  mqttLoop();  
  nrf24.NRFLoop();
  
}
