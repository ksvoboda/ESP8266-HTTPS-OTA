#define USING_AXTLS
// #include "mqtt.h"
#include <ArduinoJson.h>
#include "tcpCleanup.h"
#include <ESP8266WiFi.h>
#include "ESP8266httpUpdate.h"
#include <WiFiClientSecureBearSSL.h>

#include <WiFiClientSecureAxTLS.h>
using namespace axTLS;

#include <ESP8266WiFi.h>
#include <MQTT.h>

const char* mqttServer = "192.168.12.131";
const int mqttPort = 1883;
const char* mqttUser = "guest";
const char* mqttPassword = "guest";

const int httpsPort = 443;
String downloadLink = "https://api.github.com/repos/ksvoboda/ESP8266-HTTPS-OTA/releases/latest";
const char* token = "062c3bc11cbf5b2c0268ca214b245a0f2ba1fefa";

String currentFWVersion = "v1.0.6";

WiFiClientSecure wifiClientSecure;
HTTPClient httpClient;

WiFiClient wifiClient;
MQTTClient mqttClient;

unsigned long lastMillis = 0;

void firmwareUpdate() {

  //-------------Getting download URL-------------//
  
  httpClient.addHeader("Authorization: token", token);
  httpClient.begin(wifiClientSecure, downloadLink);

  int httpCode = httpClient.GET();

  if (httpCode > 0) {

    Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
    
  } else {
    
    Serial.printf("[HTTPS] GET... failed, error: %s\n", httpClient.errorToString(httpCode).c_str());
    
  }

  String APIReply = httpClient.getString();
  Serial.println("GitHub API reply is: " + APIReply);

  DynamicJsonDocument doc(4096);
  deserializeJson(doc, APIReply);

  String fwVersion = doc["tag_name"];
  String fullDownloadURL = doc["assets"][0]["browser_download_url"];

  Serial.print("Latest FW version: ");
  Serial.println(fwVersion);
  Serial.print("Latest FW download URL: ");
  Serial.println(fullDownloadURL);

  //-------------OTA Update-------------//

  wifiClientSecure.stop();
  yield();
  tcpCleanup();

  if(currentFWVersion != fwVersion) {

    Serial.print("updating...");
    // OTA HTTPS Update from GitHub latest release
    ESPhttpUpdate.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);
    auto ret = ESPhttpUpdate.update(wifiClientSecure, fullDownloadURL);

    Serial.println("Update failed");
    Serial.println((int) ret);
  
  } else {

    Serial.println("firmware is already up to date.");
    
  }

}

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
