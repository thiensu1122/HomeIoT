#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <ArduinoJson.h>



#define MQTTINTERVAL  1000
#define MQTTHUB "hub"
#define MQTTSERVER "server"
#define MQTTDEVICE "device"

#define UPDATEALLSENSOR 50
#define ALLSENSORMESSAGE "ALL SENSOR"
#define HUBTOSERVER "0"
#define TEMPSENSOR "52"
#define CO2SENSOR "53"



void connectMQTT();
void checkMQTTConnect();
void publicMessage(String data);
String createMessage();

void statusLoop();
void callback(char* topic, byte* payload, unsigned int length);
void mqttLoop();
