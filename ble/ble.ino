// ble.ino
#include <SPI.h>
#include <EEPROM.h>
#include "aci.h"
#include "utils.h"
#include "blue_cap_peripheral.h"
#include "peripheral.h"

#define REQN_PIN              9
#define RDYN_PIN              8
#define EEPROM_OFFSET         0
#define CLEAR_BONDS_PIN       6
#define ADD_BOND_PIN          7
#define MAX_BONDS             4

Peripheral peripheral(REQN_PIN, RDYN_PIN, EEPROM_OFFSET, MAX_BONDS);

void setup() {
  peripheral.begin();
}

void loop() {
  peripheral.loop();
}

