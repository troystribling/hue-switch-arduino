#include <SPI.h>
#include <string.h>
#include "Adafruit_CC3000.h"
#include "ccspi.h"
#include "utils.h"
#include "hue_lights_client.h"
#include "private.h"

HueLightsClient client(WEB_HOST, WEB_ROOT);

void setup(void) {
  Serial.begin(9600);
  DBUG_FREE_MEMORY;
  client.lanConnect(WLAN_SSID, WLAN_PASS);
  if (client.lanDisconnect()) {
    INFO_LOG(F("Disconnected from LAN"));
  }
}

void loop(void) {
 delay(1000);
}
