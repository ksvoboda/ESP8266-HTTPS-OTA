#include "firmwareUpdate.h"

const char* ssid = "curry";
const char* password = "Papagaj43*";

void setup() {

  delay(3500);

  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);

  //Setting up WiFi and connecting
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);
    Serial.print(".");

  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  firmwareUpdate();

}

void loop() {

}
