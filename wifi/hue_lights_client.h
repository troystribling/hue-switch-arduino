#ifndef _HUE_LIGHTS_CLIENT_H_
#define _HUE_LIGHTS_CLIENT_H_

#include "Adafruit_CC3000.h"

class HueLightsClient {

public:

  HueLightsClient(char*       _host,
                  char*       _site_root);

  bool lanConnect(const char*  _wlan_ssid, const char*  _wlan_password);
  bool lanConnected();
  bool lanDisconnect();

  bool setLightOn(uint8_t light, bool on);

private:

  void init(const char* _wlan_ssid, const char* _wlan_password);
  bool siteConnected();
  bool siteConnect();
  bool siteClose();
  bool displayConnectionDetails();
  bool httpRequest(const __FlashStringHelper* method, char* url, char* headers, char* body);
  bool readHTTPResponse();

private:

  uint32_t                    serverIpAddress;
  char*                       host;
  char*                       site_root;
  Adafruit_CC3000             cc3000;
  Adafruit_CC3000_Client      client;

};

#endif