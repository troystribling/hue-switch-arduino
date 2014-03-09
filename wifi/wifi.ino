#include <SPI.h>
#include <string.h>
#include "Adafruit_CC3000.h"
#include "ccspi.h"
#include "utils.h"
#include "hue_lights_client.h"
#include "private.h"

void setup(void) {
  Serial.begin(9600);
  DBUG_FREE_MEMORY
}

void loop(void) {
 delay(1000);
}
