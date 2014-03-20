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
#define SET_CURRENT_SCENE_CMD       12
#define GET_LIGHT_COLOR_CMD         13
#define SET_LIGHT_COLOR_CMD         14
#define GET_LIGHT_COUNT             15
#define SET_LIGHT_COUNT             16
#define GET_SCENE_COUNT             17

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
  MENU("10. Get Scene");
  MENU("11. Get Current Scene ID");
  MENU("12. Set Current Scene ID");
  MENU("13. Get Light Color");
  MENU("14. Set Light Color");
  MENU("15. Get Light Count");
  MENU("16. Set Light Count");
  MENU("17. Get Scene Count");
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
  MENU("\nSet All Lights On: OnState-");
}

void SerialUI::showSetSceneNameMenu() {
  MENU("\nSet Scene Name: Name-");
}

void SerialUI::showGetSceneMenu() {
  MENU("\nGet Scene: ID-");
}

void SerialUI::showRemoveSceneMenu() {
  MENU("\nSet All Lights: OnState");
}

void SerialUI::showSetCurrentSceneMenu() {
  MENU("\nSet Current Scene: ID-");
}

void SerialUI::showSetLightColorMenu() {
  MENU("\nSet Light Color: LightId&Brightness&Saturation&Hue-");
}

void SerialUI::showGetLightColorMenu() {
  MENU("\nGet Light Color: ID-");
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
        showRemoveSceneMenu();
        break;
      case SET_SCENE_NAME_CMD:
        showSetSceneNameMenu();
        break;
      case GET_SCENE_CMD:
        showGetSceneMenu();
        break;
      case SET_CURRENT_SCENE_CMD:
        showSetCurrentSceneMenu();
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
  if (client->setLightColor(lightID, light)) {
    scene.lights[lightID].brightness = light.brightness;
    scene.lights[lightID].saturation = light.saturation;
    scene.lights[lightID].hue = light.hue;
  }
}

void SerialUI::processGetLightColor() {
  uint8_t lightID = (uint8_t)atoi(messageBuffer[0]);
  DBUG_LOG(F("brightness:"));
  DBUG_LOG(scene.lights[lightID].brightness, DEC);
  DBUG_LOG(F("saturation:"));
  DBUG_LOG(scene.lights[lightID].saturation, DEC);
  DBUG_LOG(F("hue:"));
  DBUG_LOG(scene.lights[lightID].hue, DEC);
}

void  SerialUI::processGetScene() {
  sceneID = (uint8_t)atoi(messageBuffer[0]);
  DBUG_LOG(F("sceneID:"));
  DBUG_LOG(sceneID);
  client->getScene(sceneID, scene);
  DBUG_LOG(F("sceneID:"));
  DBUG_LOG(sceneID);
  DBUG_LOG(F("Scene Name:"));
  DBUG_LOG(scene.name);
  for (int i = 0; i < client->setLightCount(); i++) {
    DBUG_LOG(F("lightID:"));
    DBUG_LOG(i);
    DBUG_LOG(F("brightness:"));
    DBUG_LOG(scene.lights[i].brightness);
    DBUG_LOG(F("saturation:"));
    DBUG_LOG(scene.lights[i].saturation);
    DBUG_LOG(F("hue:"));
    DBUG_LOG(scene.lights[i].hue);
  }
}

void  SerialUI::processSetSceneName() {
  char* sceneName = messageBuffer[0];
  DBUG_LOG(F("sceneName:"));
  DBUG_LOG(sceneName);
  strcpy(scene.name, sceneName);
}

void SerialUI::processSetCurrentScene() {
  uint8_t sceneID = (uint8_t)atoi(messageBuffer[0]);
  DBUG_LOG(F("sceneID:"));
  DBUG_LOG(sceneID);
  client->setCurrentScene(sceneID);
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
        client->createScene(scene);
        break;
      case UPDATE_SCENE_CMD:
        client->updateScene(sceneID, scene);
        break;
      case REMOVE_SCENE_CMD:
        client->removeScene(sceneID);
        break;
      case NEXT_SCENE:
        client->nextScene(sceneID, scene);
        break;
      case GET_SCENE_NAME_CMD:
        DBUG_LOG(F("Scene Name:"));
        DBUG_LOG(scene.name);
        break;
      case SET_SCENE_NAME_CMD:
        processSetSceneName();
        break;
      case GET_SCENE_ID_CMD:
        DBUG_LOG(F("Scene ID:"));
        DBUG_LOG(sceneID);
        break;
      case GET_SCENE_CMD:
        processGetScene();
        break;
      case GET_CURRENT_SCENE_ID_CMD:
        client->getCurrentSceneID();
        break;
      case SET_CURRENT_SCENE_CMD:
        processSetCurrentScene();
        break;
      case GET_LIGHT_COLOR_CMD:
        processGetLightColor();
        break;
      case SET_LIGHT_COLOR_CMD:
        processSetLightColor();
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

