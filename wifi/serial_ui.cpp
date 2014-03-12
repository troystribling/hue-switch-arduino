#include "serial_ui.h"
#include "log.h"

#define MENU(X) Serial.println(F(X))

#define MAIN_MENU_CMD               0
#define LIGHT_ON_CMD                1
#define ALL_LIGHTS_ON_CMD           2
#define ADD_SCENE_CMD               3
#define REMOVE_SCENE_CMD            4
#define NEXT_SCENE                  5
#define NEXT_LIGHT                  6
#define GET_SCENE_NAME_CMD          7
#define SET_SCENE_NAME_CMD          8
#define GET_SCENE_ID_CMD            9
#define SET_SCENE_ID_CMD            10
#define GET_CURRENT_SCENE_ID_CMD    11
#define SET_CURRENT_SCENE_ID_CMD    12
#define GET_LIGHT_COLOR_CMD         13
#define SET_LIGHT_COLOR_CMD         14
#define SHOW_SCENES                 15
#define GET_LIGHT_COUNT             16

void SerialUI::showMainMenu() {
  MENU("\nHue Lights Main Menu");
  MENU("Terminate entry with -");
  MENU("1.  Set Light On State");
  MENU("2.  Set All Lights On State");
  MENU("3.  Add Scence");
  MENU("4.  Remove Scene");
  MENU("5.  Next Scene");
  MENU("6.  Next Light");
  MENU("7.  Get Scene Name");
  MENU("8.  Set Scene Name");
  MENU("9.  Get Scene ID");
  MENU("10. Set Scene ID");
  MENU("11. Get Current Scene ID");
  MENU("12. Set Current Scene ID");
  MENU("13. Get Light Color");
  MENU("14. Set Light Color");
  MENU("15. Show Scenes");
  MENU("16. Get Light Count");
}

void SerialUI::processSerialInput() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c == '-' || currentBufferIndex == MAX_MESSAGE_SIZE) {
      messageBuffer[currentBufferIndex] = '\0';
      if (currentCommandID == 0) {
        currentCommandID = atoi(messageBuffer);
        showMenu();
      } else {
        processCommand(messageBuffer, currentBufferIndex);
        currentCommandID = 0;
        showMainMenu();
      }
      currentBufferIndex = 0;
    } else if (c == '&') {
      messageBuffer[currentBufferIndex] = '\0';
      currentBufferIndex++;
    } else {
      messageBuffer[currentBufferIndex] = c;
      currentBufferIndex++;
    }
  }
}

void SerialUI::showSetLightOnMenu() {
  MENU("\nSet Light On: LightId&OnState");
}

void SerialUI::showSetAllLightsOnMenu() {
  MENU("\nSet All Lights On: OnState");
}

void SerialUI::showSetLightColorMenu() {
}

void SerialUI::showAddSceneMenu() {
}

void SerialUI::showRemoveceneMenu() {
}

void SerialUI::showGetSceneNameMenu() {
}

void SerialUI::showSetSceneNameMenu() {
}

void SerialUI::showGetSceneIDMenu() {
}

void SerialUI::showSetSceneIDMenu() {
}

void SerialUI::showSetCurrentSceneIDMenu() {
}

void SerialUI::showGetLightColorMenu() {
}

void SerialUI::showSetLightColorenu() {
}

void SerialUI::showMenu() {
  DBUG_LOG(F("SerialUI::showMenu"));
  DBUG_LOG(currentCommandID, DEC);
  switch (currentCommandID) {
      case MAIN_MENU_CMD:
        showMainMenu();
        break;
      case LIGHT_ON_CMD:
        showSetLightOnMenu();
        break;
      case ALL_LIGHTS_ON_CMD:
        showSetAllLightsOnMenu();
        break;
      case ADD_SCENE_CMD:
        showAddSceneMenu();
        break;
      case REMOVE_SCENE_CMD:
        showRemoveceneMenu();
        break;
      case GET_SCENE_NAME_CMD:
        showGetSceneNameMenu();
        break;
      case SET_SCENE_NAME_CMD:
        showSetSceneNameMenu();
        break;
      case GET_SCENE_ID_CMD:
        showGetSceneIDMenu();
        break;
      case SET_SCENE_ID_CMD:
        showSetSceneIDMenu();
        break;
      case SET_CURRENT_SCENE_ID_CMD:
        showSetCurrentSceneIDMenu();
        break;
      case GET_LIGHT_COLOR_CMD:
        showGetLightColorMenu();
        break;
      case SET_LIGHT_COLOR_CMD:
        showSetLightColorenu();
        break;
      case NEXT_SCENE:
      case NEXT_LIGHT:
      case GET_CURRENT_SCENE_ID_CMD:
      case SHOW_SCENES:
      case GET_LIGHT_COUNT:
        processCommand(NULL, 0);
        break;
      default:
        ERROR_LOG(F("(SerialUI::showMenu) Command ID is invalid:"));
        ERROR_LOG(currentCommandID, DEC);
        break;
  }
}


void SerialUI::processLightOn(char* data, uint8_t size) {
  DBUG_LOG(F("SerialUI::processLightOn"));
  uint8_t lightID = (uint8_t)atoi(&data[0]);
  bool lightOnStatus = (bool)atoi(&data[2]);
  DBUG_LOG(F("lightID:"));
  DBUG_LOG(lightID, DEC);
  DBUG_LOG(F("lightOnStatus:"));
  DBUG_LOG(lightOnStatus, DEC);
  client->setLightOn(lightID, lightOnStatus);
}

void SerialUI::processCommand(char* data, uint8_t size) {
  DBUG_LOG(F("SerialUI::processCommand"));
  DBUG_LOG(currentCommandID, DEC);
  switch (currentCommandID) {
      case LIGHT_ON_CMD:
        processLightOn(data, size);
        break;
      case ALL_LIGHTS_ON_CMD:
        break;
      case ADD_SCENE_CMD:
        break;
      case REMOVE_SCENE_CMD:
        break;
      case NEXT_SCENE:
        break;
      case NEXT_LIGHT:
        break;
      case GET_SCENE_NAME_CMD:
        break;
      case SET_SCENE_NAME_CMD:
        break;
      case GET_SCENE_ID_CMD:
        break;
      case SET_SCENE_ID_CMD:
        break;
      case GET_CURRENT_SCENE_ID_CMD:
        break;
      case SET_CURRENT_SCENE_ID_CMD:
        break;
      case GET_LIGHT_COLOR_CMD:
        break;
      case SET_LIGHT_COLOR_CMD:
        break;
      case SHOW_SCENES:
        break;
      case GET_LIGHT_COUNT:
        break;
      default:
        ERROR_LOG(F("(SerialUI::processCommand) Command ID is invalid:"));
        ERROR_LOG(currentCommandID, DEC);
        break;
  }
}
