#ifndef _SERIAL_INTERFACE_H
#define _SERIAL_INTERFACE_H

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

#define MAX_MESSAGE_SIZE            20
uint8_t currentCommand = 0;
uint8_t messageBuffer[MAX_MESSAGE_SIZE];

void showMainMenu() {
  MENU("\nHue Lights Main Menue");
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
  MENU("11. Get Light Color");
  MENU("12. Set Light Color");
  MENU("13. Show Scenes");
}

void showSetLightOnMenu() {
}

void showSetAllLightsOnMenu() {
}

void showSetLightColorMenu() {
}

void showAddSceneMenu() {
}

void showRemoveceneMenu() {
}

void showGetSceneNameMenu() {
}

void showSetSceneNameMenu() {
}

void showGetSceneIDMenu() {
}

void showSetSceneIDMenu() {
}

void showSetCurrentSceneIDMenu() {
}

void showGetLightColorMenu() {
}

void showSetLightColorenu() {
}

void processCommand(uint8_t commandId, uint8_t* data) {
  switch (commandId) {
      case MAIN_MENU_CMD:
        break;
      case LIGHT_ON_CMD:
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
  }
}

void showMenu(uint8_t commandId) {
  switch (commandId) {
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
        processCommand(commandId, NULL);
        break;
  }
}

void processSerialInput() {
  if (Serial.available()) {
    char c = Serial.read();
    Serial.println(c);
  }
}

#endif