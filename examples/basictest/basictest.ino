#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "TzIpGeolocation.h"

#define MY_KEY "MY KEY to be generated on ipgeolocation.io"
#define SSID "MY WIFI NETWORK"
#define PASSWORD "MY PASSWORD"

TimeZoneIpGeolocation timeZone(MY_KEY);


void setup() {
  Serial.begin(115200);

  Serial.print("Attempting to connect to SSID: ");
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nConnected");

  Serial.println("Set root CA");
  timeZone.begin();
  Serial.println("Request server");
  timeZone.request();
  
  Serial.println(timeZone.getTimeZone());
  Serial.println(timeZone.getOffset());
  Serial.println(timeZone.getPubIP());
  Serial.println(timeZone.getIsDst());
  Serial.println(timeZone.getDstSavings());
  Serial.println(timeZone.getCountryCode2());
  Serial.println(timeZone.getCountryCode3());

  
}

void loop() {
  // do nothing
}
