#include <RH_NRF24.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include "DataPackage.h"
#include "NRF24Message.h"
#include "NRF24.h"

 
String apiKey = "KBD1JSZTUKCXJ15V";
 
const char* ssid = "Bao2G";
const char* password = "bao0123456";
 
WiFiClient client;
int gatewayID = EEPROM.read(0);
 
const char* server = "api.thingspeak.com";

//MISO connects to pin D6 of the NodeMCU
//MOSI connects to pin D7 of the NodeMCU
//SCK connects to pin D5 of the NodeMCU
//CE connects to pin D4 of the NodeMCU
//CSN connects to pin D2 of the NodeMCU
RH_NRF24 nrf24(2, 4); // use this for NodeMCU Amica/AdaFruit Huzzah ESP8266 Feather
 
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
  setupNRF(nrf24);
}
 
void loop()
{
  if (nrf24.available())
  {
    Serial.println("something");
    // Should be a message for us now
    uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(DataPackage);
    memset(buf,0,sizeof(buf));
    if (nrf24.recv(buf, &len))
    {
      for(int i = 0 ;i< len;i++){
        Serial.print(buf[i]);
        Serial.print("|");
      }
      
      // Send a reply
      uint8_t sdata[] = "Data Received.";
      nrf24.send(sdata, sizeof(sdata));
      nrf24.waitPacketSent();
      DataPackage dataPackage;
      memcpy( &dataPackage, buf, sizeof( DataPackage ) );
      NRF24Message nrf24Message(dataPackage);
      nrf24Message.printData();
      
    }
  } 
  else 
  {
 
    //Serial.print("No New Message");
  }
//delay(15000);
}
