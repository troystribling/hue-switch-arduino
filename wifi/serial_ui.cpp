#include "serial_ui.h"
#include "log.h"

#define MENU(X) Serial.println(F(X))

#define MAIN_MENU_CMD               0
#define LIGHT_ON_CMD                1
#define ALL_LIGHTS_ON_CMD           2
#define ADD_SCENE_CMD               3
#define REMOVE_SCENE_CMD            4
#define NEXT_SCENE                  5
#define GET_SCENE_NAME_CMD          6
#define SET_SCENE_NAME_CMD          7
#define GET_SCENE_ID_CMD            8
#define SET_SCENE_ID_CMD            9
#define GET_CURRENT_SCENE_ID_CMD    10
#define SET_CURRENT_SCENE_ID_CMD    11
#define GET_LIGHT_COLOR_CMD         12
#define SET_LIGHT_COLOR_CMD         13
#define SHOW_SCENES                 14
#define GET_LIGHT_COUNT             15
#define SET_LIGHT_COUNT             16

void SerialUI::showMainMenu() {
  currentCommandID = 0;
  MENU("\nHue Lights Main Menu");
  MENU("Terminate entry with -");
  MENU("1.  Set Light On State");
  MENU("2.  Set All Lights On State");
  MENU("3.  Add Scence");
  MENU("4.  Remove Scene");
  MENU("5.  Next Scene");
  MENU("6.  Get Scene Name");
  MENU("7.  Set Scene Name");
  MENU("8.  Get Scene ID");
  MENU("9.  Set Scene ID");
  MENU("10. Get Current Scene ID");
  MENU("11. Set Current Scene ID");
  MENU("12. Get Light Color");
  MENU("13. Set Light Color");
  MENU("14. Show Scenes");
  MENU("15. Get Light Count");
  MENU("16. Set Light Count");
}

void SerialUI::processSerialInput() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c == '-' || currentBufferIndex == MAX_MESSAGE_SIZE) {
      messageBuffer[currentMessageIndex][currentBufferIndex] = '\0';
      if (currentCommandID == 0) {
        currentCommandID = atoi(messageBuffer[0]);
        showMenu();
      } else {
        processCommand();
        showMainMenu();
      }
      currentBufferIndex = 0;
      currentMessageIndex = 0;
    } else if (c == '&') {
      messageBuffer[currentMessageIndex][currentBufferIndex] = '\0';
      currentMessageIndex++;
      currentBufferIndex = 0;
    } else {
      messageBuffer[currentMessageIndex][currentBufferIndex] = c;
      currentBufferIndex++;
    }
  }
}

void SerialUI::showSetLightOnMenu() {
  MENU("\nSet Light On: LightId&OnState-");
}

void SerialUI::showSetAllLightsOnMenu() {
  MENU("\nSet All Lights On: OnState");
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

void SerialUI::showSetLightColorMenu() {
  MENU("\nSet Light Color: LightId&Brightness&Saturation&Hue-");
}

void SerialUI::showMenu() {
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
        showSetLightColorMenu();
        break;
      case NEXT_SCENE:
      case GET_CURRENT_SCENE_ID_CMD:
      case SHOW_SCENES:
      case GET_LIGHT_COUNT:
      case SET_LIGHT_COUNT:
        processCommand();
        showMainMenu();
        break;
      default:
        ERROR_LOG(F("(SerialUI::showMenu) Command ID is invalid:"));
        ERROR_LOG(currentCommandID, DEC);
        showMainMenu();
        break;
  }
}


void SerialUI::processSetLightOn() {
  DBUG_LOG(F("SerialUI::processLightOn"));
  uint8_t lightID = (uint8_t)atoi(messageBuffer[0]);
  bool lightOnStatus = (bool)atoi(messageBuffer[1]);
  DBUG_LOG(F("lightID:"));
  DBUG_LOG(lightID, DEC);
  DBUG_LOG(F("lightOnStatus:"));
  DBUG_LOG(lightOnStatus, DEC);
  client->setLightOn(lightID, lightOnStatus);
}

void SerialUI::processSetLightColor() {
  DBUG_LOG(F("SerialUI::processSetLightColor"));
  HueLight light;
  uint8_t lightID = (uint8_t)atoi(messageBuffer[0]);
  light.brightness = (uint8_t)atoi(messageBuffer[1]);
  light.saturation = (uint8_t)atoi(messageBuffer[2]);
  light.hue = (uint16_t)atoi(messageBuffer[3]);
  DBUG_LOG(F("lightID:"));
  DBUG_LOG(lightID, DEC);
  DBUG_LOG(F("brightness:"));
  DBUG_LOG(light.brightness, DEC);
  DBUG_LOG(F("saturation:"));
  DBUG_LOG(light.saturation, DEC);
  DBUG_LOG(F("hue:"));
  DBUG_LOG(light.hue, DEC);
  client->setLightColor(lightID, light);
}

void SerialUI::processCommand() {
  switch (currentCommandID) {
      case LIGHT_ON_CMD:
        processSetLightOn();
        break;
      case ALL_LIGHTS_ON_CMD:
        break;
      case ADD_SCENE_CMD:
        break;
      case REMOVE_SCENE_CMD:
        break;
      case NEXT_SCENE:
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
        processSetLightColor();
        break;
      case SHOW_SCENES:
        break;
      case GET_LIGHT_COUNT:
        client->getLightCount();
        break;
      case SET_LIGHT_COUNT:
        client->setLightCount();
        break;
      default:
        ERROR_LOG(F("(SerialUI::processCommand) Command ID is invalid:"));
        ERROR_LOG(currentCommandID, DEC);
        showMainMenu();
        break;
  }
}

