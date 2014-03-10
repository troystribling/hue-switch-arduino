#include <SPI.h>
#include <EEPROM.h>
#include "Adafruit_CC3000.h"
#include "ccspi.h"
#include "utils.h"
#include "hue_lights_client.h"
#include "private.h"

HueLightsClient client(WEB_HOST, WEB_ROOT);


#ifdef SERIAL_INTERFACE

#define MENU(X) Serial.println(F(X))

#define MAIN_MENU_CMD          0
#define LIGHT_ON_CMD           1
#define ALL_LIGHTS_ON_CMD      2
#define LIGHT_COLOR_CMD        3
#define ADD_SCENE_CMD          4
#define ADD_SCENE_LIGHT_CMD    5
#define SET_SCENE_CMD          6
#define DELETE_SCENE_CMD       7
#define LIST_SCENES_CMD        8

#define MAX_MESSAGE_SIZE    20

uint8_t currentCommand = 0;

char messageBuffer[MAX_MESSAGE_SIZE];

void showMainMenu() {
  MENU("\nHue Lights Main Menue");
  MENU("1. Set Light On State");
  MENU("2. Set All Lights On State");
  MENU("3. Set Light Color");
  MENU("4. Add Scene");
  MENU("5. Add Scene Light");
  MENU("6. Set Scene");
  MENU("7. Delete Scene");
  MENU("8. Show Scenes");
}

void showSetLightOnMenu() {
}

void showSetAllLightsOnMenu() {
}

void showSetLightColorMenu() {
}

void showAddSceneMenu() {
}

void showSetSceneMenu() {
}

void showDeleteSceneMenu() {
}

void processCommand(uint8_t commandId) {
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
      case LIGHT_COLOR_CMD:
        showSetLightColorMenu();
        break;
      case ADD_SCENE_CMD:
        showAddSceneMenu();
        break;
      case ADD_SCENE_LIGHT_CMD:
        break;
      case SET_SCENE_CMD:
        showSetSceneMenu();
        break;
      case DELETE_SCENE_CMD:
        showDeleteSceneMenu();
        break;
      case LIST_SCENES_CMD:
        break;
  }
}

#endif
void setup() {
  Serial.begin(9600);
  DBUG_FREE_MEMORY;
  client.lanConnect(WLAN_SSID, WLAN_PASS);
#ifdef SERIAL_INTERFACE
  showMainMenu();
#endif
}

void loop() {
#ifdef SERIAL_INTERFACE
  if (Serial.available()) {
    char c = Serial.read();
    Serial.println(c);
  }
#else
#endif
}
