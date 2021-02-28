#define USING_AXTLS
#include <ArduinoJson.h>
#include "tcpCleanup.h"
#include <ESP8266WiFi.h>
#include "ESP8266httpUpdate.h"
#include <WiFiClientSecureBearSSL.h>

#include <WiFiClientSecureAxTLS.h>
using namespace axTLS;

const int httpsPort = 443;
String downloadLink = "https://api.github.com/repos/ksvoboda/ESP8266-HTTPS-OTA/releases/latest";
const char* token = "062c3bc11cbf5b2c0268ca214b245a0f2ba1fefa";

String currentFWVersion = "v1.0.6";

WiFiClientSecure wifiClientSecure;
HTTPClient httpClient;

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
