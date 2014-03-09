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

uint8_t currentCommand = 0;

void showMainMenu() {
  MENU("\nHue Lights Main Menue");
  MENU("1. Set Light On State");
  MENU("2. Set All Lights On State");
  MENU("3. Set Light Color");
  MENU("4. Create Scene");
  MENU("5. Set Scene");
  MENU("6. Delete Scene");
  MENU("7. Show Scenes");
}

void showSetLightOnMenu() {
}

void showSetAllLightsOnMenu() {
}

void showSetLightColorMenuw() {
}

void showCreateSceneMenue() {
}

void showSetSceneMenu() {
}

void showDeleteSceneMenu() {
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
  }
#else
#endif
}
