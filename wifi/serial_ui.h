#ifndef _SERIAL_UI_H
#define _SERIAL_UI_H

#import "hue_lights_client.h"

#define MAX_MESSAGE_SIZE    6
#define MAX_MESSAGES        5

class SerialUI {

public:

  SerialUI(HueLightsClient* _client);
  void showMainMenu();
  void processSerialInput();

private:

  void showSetLightOnMenu();
  void showSetAllLightsOnMenu();
  void showSetLightColorMenu();
  void showAddSceneMenu();
  void showRemoveSceneMenu();
  void showSetSceneNameMenu();
  void showGetSceneIDMenu();
  void showGetSceneMenu();
  void showSetCurrentSceneMenu();
  void showGetLightColorMenu();
  void showSetLightColorenu();
  void showMenu();

  void processSetLightOn();
  void processSetAllLightsOn();
  void processSetLightColor();
  void processGetLightColor();
  void processGetScene();
  void processSetSceneName();
  void processSetCurrentScene();
  void processEraseEEPROM();
  void processCommand();

private:

  HueLightsClient*  client;
  uint8_t           currentCommandID;
  uint8_t           currentBufferIndex;
  uint8_t           currentMessageIndex;
  char              messageBuffer[MAX_MESSAGES][MAX_MESSAGE_SIZE];
  HueLightsScene    scene;
  uint8_t           sceneID;
};

#endif