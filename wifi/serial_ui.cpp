#include "serial_ui.h"
#include "log.h"

#define MENU(X) Serial.println(F(X))

#define MAIN_MENU_CMD               0
#define LIGHT_ON_CMD                1
#define ALL_LIGHTS_ON_CMD           2
#define CREATE_SCENE_CMD            3
#define UPDATE_SCENE_CMD            4
#define REMOVE_SCENE_CMD            5
#define NEXT_SCENE                  6
#define GET_SCENE_NAME_CMD          7
#define SET_SCENE_NAME_CMD          8
#define GET_SCENE_ID_CMD            9
#define GET_SCENE_CMD               10
#define GET_CURRENT_SCENE_ID_CMD    11
#define SET_CURRENT_SCENE_ID_CMD    12
#define GET_LIGHT_COLOR_CMD         13
#define SET_LIGHT_COLOR_CMD         14
#define SHOW_SCENES                 15
#define GET_LIGHT_COUNT             16
#define SET_LIGHT_COUNT             17
#define GET_SCENE_COUNT             18

void SerialUI::showMainMenu() {
  currentCommandID = 0;
  MENU("\nHue Lights Main Menu");
  MENU("Terminate entry with -");
  MENU("1.  Set Light On State");
  MENU("2.  Set All Lights On State");
  MENU("3.  Create Scence");
  MENU("4.  Update Scence");
  MENU("5.  Remove Scene");
  MENU("6.  Next Scene");
  MENU("7.  Get Scene Name");
  MENU("8.  Set Scene Name");
  MENU("9.  Get Scene ID");
  MENU("10.  Get Scene");
  MENU("11. Get Current Scene ID");
  MENU("12. Set Current Scene ID");
  MENU("13. Get Light Color");
  MENU("14. Set Light Color");
  MENU("15. Show Scenes");
  MENU("16. Get Light Count");
  MENU("17. Set Light Count");
  MENU("18. Get Scene Count");
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

void SerialUI::showRemoveceneMenu() {
}

void SerialUI::showSetSceneNameMenu() {
}

void SerialUI::showGetSceneMenu() {
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
      case REMOVE_SCENE_CMD:
        showRemoveceneMenu();
        break;
      case SET_SCENE_NAME_CMD:
        showSetSceneNameMenu();
        break;
      case GET_SCENE_CMD:
        showGetSceneMenu();
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
      case CREATE_SCENE_CMD:
      case UPDATE_SCENE_CMD:
      case GET_SCENE_NAME_CMD:
      case GET_SCENE_ID_CMD:
      case NEXT_SCENE:
      case GET_CURRENT_SCENE_ID_CMD:
      case SHOW_SCENES:
      case GET_LIGHT_COUNT:
      case SET_LIGHT_COUNT:
      case GET_SCENE_COUNT:
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

void SerialUI::processSetAllLightsOn() {
  bool lightOnStatus = (bool)atoi(messageBuffer[0]);
  DBUG_LOG(F("lightOnStatus:"));
  DBUG_LOG(lightOnStatus, DEC);
  client->setAllLightsOn(lightOnStatus);
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
        processSetAllLightsOn();
        break;
      case CREATE_SCENE_CMD:
        break;
      case UPDATE_SCENE_CMD:
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
      case GET_SCENE_CMD:
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
      case GET_SCENE_COUNT:
        client->getSceneCount();
        break;
      default:
        ERROR_LOG(F("(SerialUI::processCommand) Command ID is invalid:"));
        ERROR_LOG(currentCommandID, DEC);
        showMainMenu();
        break;
  }
}

