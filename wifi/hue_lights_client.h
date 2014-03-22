#ifndef _HUE_LIGHTS_CLIENT_H_
#define _HUE_LIGHTS_CLIENT_H_

#include "Adafruit_CC3000.h"
#include "eeprom_objects.h"

// eeprom storage objects
#define NUMBER_OF_LIGHTS_OFFSET           0
#define MAX_NUMBER_OF_LIGHTS              1
struct NumberOfLights {
  uint8_t status;
  uint8_t numberOfLights;
};

#define HUE_CURRENT_SCENE_ID_OFFSET           2
#define MAX_NUMBER_OF_HUE_CURRENT_ID_SCENES   1
struct HueLightsCurrentSceneID {
  uint8_t status;
  uint8_t currentSceneID;
};

#define HUE_LIGHTS_SCENES_OFFSET          4
#define HUE_LIGHTS_MAX_SCENES             12
#define HUE_LIGHTS_SCENE_MAX_LIGHTS       10
struct HueLight {
  uint8_t brightness;
  uint8_t saturation;
  uint16_t hue;
};
struct HueLightsScene {
  uint8_t status;
  char name[20];
  HueLight lights[HUE_LIGHTS_SCENE_MAX_LIGHTS];
};

// HueLightsClient
class HueLightsClient {

public:

  HueLightsClient(char*       _host,
                  char*       _siteRoot);

  // network
  void lanConnect(const char*  _wlanSSID, const char*  _wlanPassword);
  bool lanConnected();
  bool lanDisconnect();

  //light commands
  bool setLightOn(uint8_t lightID, bool on);
  bool setAllLightsOn(bool on);
  bool setLightColor(uint8_t lightID, const HueLight& light);
  bool setLightCount();
  uint8_t getLightCount();

  // scene commands
  uint8_t createScene(const HueLightsScene& scene);
  void updateScene(uint8_t sceneID, const HueLightsScene& scene);
  void removeScene(uint8_t sceneID);
  void nextScene(uint8_t& sceneID, HueLightsScene& scene);
  void getScene(uint8_t sceneID, HueLightsScene& scene);
  uint8_t getSceneCount();
  uint8_t getCurrentSceneID();
  void setCurrentScene(uint8_t sceneID);

private:

  void init(const char* _wlan_ssid, const char* _wlan_password);
  bool siteConnected();
  bool siteConnect();
  bool siteClose();
  bool displayConnectionDetails();
  bool httpRequest(const __FlashStringHelper* method, String* url, String* headers, String* body);
  uint16_t readHTTPResponseStatus();
  uint8_t readHTTPLightsResponse();
  void setCurrentSceneID(uint8_t sceneID);

private:

  uint32_t                                      serverIpAddress;
  char*                                         host;
  char*                                         siteRoot;
  Adafruit_CC3000                               cc3000;
  Adafruit_CC3000_Client                        client;
  EEPROMObject<NumberOfLights>                  numberOfLightsEEPROM;
  EEPROMObject<HueLightsScene>                  scenesEEPROM;
  EEPROMObject<HueLightsCurrentSceneID>         currentSceneIDEEPROM;
};

#endif