#include <SPI.h>
#include <EEPROM.h>
#include "Adafruit_CC3000.h"
#include "ccspi.h"
#include "utils.h"
#include "hue_lights_client.h"
#include "private.h"

#ifdef SERIAL_INTERFACE
  #include "serial_interface.h"
#endif

HueLightsClient client(WEB_HOST, WEB_ROOT);

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
  processSerialInput();
#else
#endif
}
