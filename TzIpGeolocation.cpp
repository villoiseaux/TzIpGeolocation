#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "TzIpGeolocation.h"

const char*  server = "api.ipgeolocation.io";  // Server URL

char comodo_root_ca[]  = \
  "-----BEGIN CERTIFICATE-----\n" \
  "MIIDnzCCAyWgAwIBAgIQWyXOaQfEJlVm0zkMmalUrTAKBggqhkjOPQQDAzCBhTEL\n" \
  "MAkGA1UEBhMCR0IxGzAZBgNVBAgTEkdyZWF0ZXIgTWFuY2hlc3RlcjEQMA4GA1UE\n" \
  "BxMHU2FsZm9yZDEaMBgGA1UEChMRQ09NT0RPIENBIExpbWl0ZWQxKzApBgNVBAMT\n" \
  "IkNPTU9ETyBFQ0MgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwHhcNMTQwOTI1MDAw\n" \
  "MDAwWhcNMjkwOTI0MjM1OTU5WjCBkjELMAkGA1UEBhMCR0IxGzAZBgNVBAgTEkdy\n" \
  "ZWF0ZXIgTWFuY2hlc3RlcjEQMA4GA1UEBxMHU2FsZm9yZDEaMBgGA1UEChMRQ09N\n" \
  "T0RPIENBIExpbWl0ZWQxODA2BgNVBAMTL0NPTU9ETyBFQ0MgRG9tYWluIFZhbGlk\n" \
  "YXRpb24gU2VjdXJlIFNlcnZlciBDQSAyMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcD\n" \
  "QgAEAjgZgTrJaYRwWQKOqIofMN+83gP8eR06JSxrQSEYgur5PkrkM8wSzypD/A7y\n" \
  "ZADA4SVQgiTNtkk4DyVHkUikraOCAWYwggFiMB8GA1UdIwQYMBaAFHVxpxlIGbyd\n" \
  "nepBR9+UxEh3mdN5MB0GA1UdDgQWBBRACWFn8LyDcU/eEggsb9TUK3Y9ljAOBgNV\n" \
  "HQ8BAf8EBAMCAYYwEgYDVR0TAQH/BAgwBgEB/wIBADAdBgNVHSUEFjAUBggrBgEF\n" \
  "BQcDAQYIKwYBBQUHAwIwGwYDVR0gBBQwEjAGBgRVHSAAMAgGBmeBDAECATBMBgNV\n" \
  "HR8ERTBDMEGgP6A9hjtodHRwOi8vY3JsLmNvbW9kb2NhLmNvbS9DT01PRE9FQ0ND\n" \
  "ZXJ0aWZpY2F0aW9uQXV0aG9yaXR5LmNybDByBggrBgEFBQcBAQRmMGQwOwYIKwYB\n" \
  "BQUHMAKGL2h0dHA6Ly9jcnQuY29tb2RvY2EuY29tL0NPTU9ET0VDQ0FkZFRydXN0\n" \
  "Q0EuY3J0MCUGCCsGAQUFBzABhhlodHRwOi8vb2NzcC5jb21vZG9jYTQuY29tMAoG\n" \
  "CCqGSM49BAMDA2gAMGUCMQCsaEclgBNPE1bAojcJl1pQxOfttGHLKIoKETKm4nHf\n" \
  "EQGJbwd6IGZrGNC5LkP3Um8CMBKFfI4TZpIEuppFCZRKMGHRSdxv6+ctyYnPHmp8\n" \
  "7IXOMCVZuoFwNLg0f+cB0eLLUg==\n" \
  "-----END CERTIFICATE-----\n";

// Constructor
TimeZoneIpGeolocation::TimeZoneIpGeolocation(String key){
  _API_key=key;
  _pClient=new(WiFiClientSecure);
  _pClient->setCACert(comodo_root_ca);
}
TimeZoneIpGeolocation::~TimeZoneIpGeolocation(){
  delete _pClient;
}

void TimeZoneIpGeolocation::begin(){
  _pClient->setCACert(comodo_root_ca);
}


int TimeZoneIpGeolocation::request(){

  Serial.println("\nStarting connection to server...");
  if (!_pClient->connect(server, 443)){
    Serial.println("Connection failed!");
    return (0);
  }  else {
    Serial.println("Connected to server!");
    // Make a HTTP request:
    _pClient->println(String("GET /timezone?apiKey="+API_KEY+"&lang=fr HTTP/1.0"));
    _pClient->println("Host: api.ipgeolocation.io");
    _pClient->println("Connection: close");
    _pClient->println();

    while (_pClient->connected()) {
      String line = _pClient->readStringUntil('\n');
      if (line == "\r") {
        Serial.println("headers received");
        break;
      }
    }
    // if there are incoming bytes available
    // from the server, read them and print them:
    String jsonString;
    while (_pClient->available()) {
      char c = _pClient->read();
      jsonString+=c;
    }
    _pClient->stop();
    Serial.print("RAM String: ");
    Serial.println(jsonString);
    
    StaticJsonDocument<2014> doc;

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, jsonString);
    if (error){
      Serial.println(F("Failed to read json string"));    
    }
    JsonObject root = doc.as<JsonObject>();
    _timeZone=(root["timezone"] | "Region/City");
    _offset=(root["timezone_offset"] | 12);
    _pubIP=(root["geo"]["ip"] | "192.168.120.125");
    _isDst=(root["is_dst"] | false); 
    _dstSavings=(root["dst_savings"] | 12);
    _country_code2=(root["geo"]["country_code2"] | "XX");
    _country_code3=(root["geo"]["country_code3"] | "XXX");
  }

  return (1);
}

