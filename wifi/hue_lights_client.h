#ifndef _HUE_LIGHTS_CLIENT_H_
#define _HUE_LIGHTS_CLIENT_H_

#include "Adafruit_CC3000.h"

class HueLightsClient {

public:

  HueLightsClient(char*       _host,
                  char*       _site_root,
                  const char* _wlan_ssid,
                  const char* _wlan_password);

  bool connected();
  bool connect();
  bool disconnect();
  bool turnOffLight(uint8_t light);

private:

  void init(const char* _wlan_ssid, const char* _wlan_password);
  bool displayConnectionDetails();

private:

  uint32_t                    ipAddress;
  char*                       host;
  char*                       site_root;
  Adafruit_CC3000             cc3000;
  Adafruit_CC3000_Client      client;

};

#endif