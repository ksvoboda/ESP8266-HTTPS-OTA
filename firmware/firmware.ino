#include "firmwareUpdate.h"
#include <MQTT.h>

const char* mqttServer = "192.168.12.131";
const int mqttPort = 1883;
const char* mqttUser = "guest";
const char* mqttPassword = "guest";

const char* ssid = "curry";
const char* password = "Papagaj43*";

#define MSG_BUFFER_SIZE  (50)

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

  mqttClient.subscribe("/devices/update");

}

void messageReceived(String &topic, String &payload) {
  
  Serial.println("incoming: " + topic + " - " + payload);

  if (topic == "/devices/update" && payload == "update") {
    
    firmwareUpdate();
  
  }

  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.
  
}

void setup() {

  Serial.begin(115200);
  
  delay(2000);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    
    delay(500);
    Serial.print(".");
    
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  mqttClient.begin(mqttServer, wifiClient);
  mqttClient.onMessage(messageReceived);

  connect();
  
  firmwareUpdate();
  
}

void loop() {
  
  mqttClient.loop();
  
  delay(10);

  if (!mqttClient.connected()) {
    
    connect();
    
  }
  
}
