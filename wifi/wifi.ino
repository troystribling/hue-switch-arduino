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
#else
  #include <Wire.h>address
  #include "i2c_message.h"
  #include "wifi_i2c_slave.h"
  const uint8_t address = 4;
  WifiI2CSlave i2cSlave(&client, HUE_LIGHTS_I2C_ADDRESS);
#endif

void setup() {
  Serial.begin(9600);
  DBUG_FREE_MEMORY;
  client.lanConnect(WLAN_SSID, WLAN_PASS);
  client.setLightCount();
#ifdef SERIAL_UI
  ui.showMainMenu();
#else
  i2cSlave.begin();
#endif
}

void loop() {
#ifdef SERIAL_UI
  ui.processSerialInput();
#endif
}
