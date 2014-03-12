#ifndef _SERIAL_UI_H
#define _SERIAL_UI_H

#import "hue_lights_client.h"

#define MAX_MESSAGE_SIZE            25

class SerialUI {

public:

  SerialUI(HueLightsClient* _client) : client(_client), currentCommandID(0), currentBufferIndex(0){};
  void showMainMenu();
  void processSerialInput();

private:

  void showSetLightOnMenu();
  void showSetAllLightsOnMenu();
  void showSetLightColorMenu();
  void showAddSceneMenu();
  void showRemoveceneMenu();
  void showGetSceneNameMenu();
  void showSetSceneNameMenu();
  void showGetSceneIDMenu();
  void showSetSceneIDMenu();
  void showSetCurrentSceneIDMenu();
  void showGetLightColorMenu();
  void showSetLightColorenu();
  void showMenu();

  void processLightOn(char* data, uint8_t size);
  void processCommand(char* data, uint8_t size);

private:

  HueLightsClient* client;
  uint8_t currentCommandID;
  uint8_t currentBufferIndex;
  char messageBuffer[MAX_MESSAGE_SIZE];
};

#endif