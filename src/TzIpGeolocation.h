#ifndef _TIME_ZONE_LIB
#define _TIME_ZONE_LIB
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

class TimeZoneIpGeolocation{
  public:
  TimeZoneIpGeolocation(String key);
  ~TimeZoneIpGeolocation();
  
  void begin();
  int request();

  // inline acessors
  String getTimeZone() {return _timeZone;};
  int getOffset() {return _offset;};
  String getPubIP() {return _pubIP;};
  bool getIsDst() {return _isDst;};
  int getDstSavings() {return  _dstSavings;  ;};
  String getCountryCode2() {return _country_code2;};
  String getCountryCode3() {return _country_code3;};
  
  private:
  WiFiClientSecure* _pClient;
  String _API_key;

  String _timeZone;
  int _offset;
  String _pubIP;
  bool _isDst;
  int _dstSavings;  
  String _country_code2; 
  String _country_code3;
};

#endif
