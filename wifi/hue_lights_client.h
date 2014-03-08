#ifndef _HUE_LIGHTS_CLIENT_H_
#define _HUE_LIGHTS_CLIENT_H_

class HueLightsClient {

public:

  HueLightsClient(const __FlashStringHelper* _host,
                  const __FlashStringHelper* _site_root,
                  const __FlashStringHelper* _wlan_ssid,
                  const __FlashStringHelper* _wlan_password);

private:

  init(const __FlashStringHelper* _wlan_ssid, const __FlashStringHelper* _wlan_password);
  void printIpAddressAsString(ip);

private:

  uint32_t                    ipAddress;
  const __FlashStringHelper*  host;
  const __FlashStringHelper*  site_root;
  Adafruit_CC3000             cc3000;
  Adafruit_CC3000_Client      client;

};

#endif