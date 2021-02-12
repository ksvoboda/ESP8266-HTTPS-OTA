#include <ESP8266WiFi.h>
#include <PubSubClient.h>

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

const char* mqtt_server = "192.168.12.131";
const int mqttPort = 1883;
const char* mqttUser = "guest";
const char* mqttPassword = "guest";

#define MSG_BUFFER_SIZE  (50)

void callback(char* topic, byte* payload, unsigned int length) {
  
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  for (int i = 0; i < length; i++) {
    
    Serial.println((char)payload[i]);
    
  }

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    
    digitalWrite(BUILTIN_LED, LOW);
    Serial.println("new fw is ready!");

  } else {
    
    digitalWrite(BUILTIN_LED, HIGH);
    
  }

}

void reconnect() {

  while (!mqttClient.connected()) {
    
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    if (mqttClient.connect(clientId.c_str(), mqttUser, mqttPassword )) {
      
      Serial.println("connected");
      mqttClient.publish("outTopic", "hello world");
      mqttClient.subscribe("devices/update");
      
    } else {
      
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
      
    }
    
  }
  
}
