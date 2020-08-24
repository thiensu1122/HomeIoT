#include "MQTT.h"

// ------- mqtt ----- ///
const char* mqttServer = "192.168.1.15";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";
const int mqttBuffer = 512; // by default mqtt buffer is 128 bytes > 512 bytes
String HomeIotTopic = "Smarthome-1";
//init wifi client
WiFiClient espClient;
//init MQTT 
PubSubClient mqttClient(espClient);

unsigned long lastStatusSend = 0;

void connectMQTT() {
  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(callback);
  mqttClient.setBufferSize(mqttBuffer);
 
  while (!mqttClient.connected()) {
    Serial.println("Connecting to MQTT...");

    if (mqttClient.connect("ESP8266Client", mqttUser, mqttPassword )) {
      Serial.println("connected");
    } else {
      Serial.print("failed with state ");
      Serial.print(mqttClient.state());
      delay(2000);
    }
  }
  mqttClient.subscribe(HomeIotTopic.c_str());

  //[todo] make led blink for mqtt status.....
}

void checkMQTTConnect() {
  if (!mqttClient.connected()) {
    connectMQTT();
  }
}

void publicMessage(String data){
  if(mqttClient.connected()){
    mqttClient.publish(HomeIotTopic.c_str(),data.c_str());
    Serial.println(HomeIotTopic.c_str());
    Serial.println(data.c_str());
  }
}

String createMessage(){
  String message;
  // initial 200 bytes for json document.
  StaticJsonDocument<mqttBuffer> jsonDoc;
  jsonDoc["code"] = UPDATEALLSENSOR;
  jsonDoc["message"] = ALLSENSORMESSAGE;
  jsonDoc["direcionCode"] = "0";
  StaticJsonDocument<100> jsonSensor1;
  jsonSensor1["sensor_id"] = "1";
  jsonSensor1["value1"] = 21;
  jsonSensor1["value2"] = 22;
  jsonSensor1["value3"] = "23";
  StaticJsonDocument<100> jsonSensor2;
  jsonSensor2["sensor_id"] = "2";
  jsonSensor2["value1"] = 31;
  jsonSensor2["value2"] = 32;
  jsonSensor2["value3"] = "33";
  //create empty array to keep all sensor values. 
  JsonArray arraySensorData = jsonDoc.createNestedArray("sensorList");;
  arraySensorData.add(jsonSensor1);
  arraySensorData.add(jsonSensor2);
  
  serializeJsonPretty(jsonDoc, message);
  return message;
}

void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived in topic: ");
  String strpayload = "";
  String strTopic= "";
  for (int i = 0; i < strlen(topic); i++) {
    strTopic += topic[i];
  }
  Serial.println(strTopic);
  
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    //Serial.print((char)payload[i]);
    strpayload += (char)payload[i];
  }
  Serial.println();

  Serial.println(strpayload);
  
  Serial.println();
  Serial.println("-----------------------");

}

void statusLoop(){
  long timepassed = millis() - lastStatusSend;
  if(timepassed  >= MQTTINTERVAL){
    
//    Serial.print("hello every 1 sec : ");
//    Serial.println(timepassed);
    lastStatusSend = millis();

    String message = createMessage();
    publicMessage(message);
    
  }
}

void mqttLoop(){
  mqttClient.loop();
  checkMQTTConnect();
}
