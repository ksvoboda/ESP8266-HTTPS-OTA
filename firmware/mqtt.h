#include <ESP8266WiFi.h>
#include <MQTT.h>

const char* mqttServer = "192.168.12.131";
const int mqttPort = 1883;
const char* mqttUser = "guest";
const char* mqttPassword = "guest";

WiFiClient wifiClient;
MQTTClient mqttClient;

unsigned long lastMillis = 0;

void connect() {
  
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    
    Serial.print(".");
    delay(1000);
    
  }

  Serial.print("\nconnecting...");
  while (!mqttClient.connect("ESP-8266", mqttUser, mqttPassword)) {
    
    Serial.print(".");
    delay(1000);
    
  }

  Serial.println("\nconnected!");

  mqttClient.subscribe("/hello");

}

void messageReceived(String &topic, String &payload) {
  
  Serial.println("incoming: " + topic + " - " + payload);

  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.
  
}
