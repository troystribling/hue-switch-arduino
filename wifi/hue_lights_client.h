#ifndef _HUE_LIGHTS_CLIENT_H_
#define _HUE_LIGHTS_CLIENT_H_

#include "Adafruit_CC3000.h"

class HueLightsClient {

public:

  HueLightsClient(char*       _host,
                  char*       _siteRoot);

  // network
  bool lanConnect(const char*  _wlanSSID, const char*  _wlanPassword);
  bool lanConnected();
  bool lanDisconnect();

  //commands
  bool setLightOn(uint8_t lightID, bool on);
  bool setLightColor(uint8_t lightID, uint8_t saturation, uint8_t brightness, uint16_t hue);
  bool getLightCount();

private:

  void init(const char* _wlan_ssid, const char* _wlan_password);
  bool siteConnected();
  bool siteConnect();
  bool siteClose();
  bool displayConnectionDetails();
  bool httpRequest(const __FlashStringHelper* method, String* url, String* headers, String* body);
  uint16_t readHTTPResponseStatus();
  uint8_t readHTTPLightsResponse();

private:

  uint32_t                    serverIpAddress;
  char*                       host;
  char*                       siteRoot;
  Adafruit_CC3000             cc3000;
  Adafruit_CC3000_Client      client;

};

#endif