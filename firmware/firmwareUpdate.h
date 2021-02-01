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

void firmwareUpdate() {

  //-------------Getting download URL-------------//

  WiFiClientSecure client;
  HTTPClient http;
  
  http.addHeader("Authorization: token", token);
  http.begin(client, downloadLink);

  int httpCode = http.GET();

  if (httpCode > 0) {

    Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
    
  } else {
    
    Serial.printf("[HTTPS] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    
  }

  String APIReply = http.getString();
  Serial.println("GitHub API reply is: " + APIReply);

  DynamicJsonDocument doc(4096);
  deserializeJson(doc, APIReply);

  const char* fwVersion = doc["tag_name"];
  String fullDownloadURL = doc["assets"][0]["browser_download_url"];

  Serial.print("Latest firmware version: ");
  Serial.println(fwVersion);
  Serial.print("Latest firmware download URL: ");
  Serial.println(fullDownloadURL);

  //-------------OTA Update-------------//

  client.stop();
  yield();
  tcpCleanup();
  
  // OTA HTTPS Update from GitHub latest release
  ESPhttpUpdate.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);
  auto ret = ESPhttpUpdate.update(client, fullDownloadURL);

  Serial.println("Update failed");
  Serial.println((int) ret);

}
