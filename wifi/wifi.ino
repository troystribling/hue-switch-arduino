#include <SPI.h>
#include <EEPROM.h>
#include "Adafruit_CC3000.h"
#include "ccspi.h"
#include "utils.h"
#include "hue_lights_client.h"
#include "private.h"

HueLightsClient client(WEB_HOST, WEB_ROOT);

#ifdef SERIAL_UI
  #include "serial_ui.h"
  SerialUI ui(&client);
#endif

void setup() {
  Serial.begin(9600);
  DBUG_FREE_MEMORY;
  client.lanConnect(WLAN_SSID, WLAN_PASS);
  client.setLightCount();
#ifdef SERIAL_UI
  ui.showMainMenu();
#endif
}

void loop() {
#ifdef SERIAL_UI
  ui.processSerialInput();
#else
#endif
}
