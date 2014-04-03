// ble.ino
#include <SPI.h>
#include <EEPROM.h>
#include "aci.h"
#include "utils.h"
#include "blue_cap_peripheral.h"
#include "peripheral.h"

#define REQN_PIN              9
#define RDYN_PIN              8
#define CLEAR_BONDS_PIN       6
#define ADD_BOND_PIN          7
#define MAX_BONDS             4
#define ADD_BOND_PIN_COUNT    20000

uint16_t addBondCount         = 0;
bool updateAddBondCount       = false;

Peripheral peripheral(REQN_PIN, RDYN_PIN, MAX_BONDS);

void clearBond() {
  if (digitalRead(CLEAR_BONDS_PIN) == HIGH) {
    INFO_LOG(F("Clearing EEPROM bond"));
    peripheral.clearBondData();
    INFO_LOG(F("Remove wire and reset"));
    while(1){delay(1000);};
  }
}

void addBond() {
  if (digitalRead(ADD_BOND_PIN) == HIGH && addBondCount == ADD_BOND_PIN_COUNT) {
    if (peripheral.addBond()) {
      INFO_LOG(F("Added bond. Remove wire"));
    } else {
      INFO_LOG(F("Failed to add bond. Remove wire"));
    }
    addBondCount = 0;
    updateAddBondCount = false;
    delay(5000);
  } else if (digitalRead(ADD_BOND_PIN) == HIGH) {
    if (updateAddBondCount) {
      addBondCount++;
    }
  } else {
    updateAddBondCount = true;
    addBondCount = 0;
  }
}

void setup() {
  Serial.begin(9600);
  DBUG_FREE_MEMORY;
  peripheral.begin();
  pinMode(CLEAR_BONDS_PIN, INPUT);
  pinMode(ADD_BOND_PIN, INPUT);
  INFO_LOG(F("To delete the bond stored in EEPROM, connect pin 6 to 5v and reset."));
  INFO_LOG(F("To add a bond connect pin 7 to 5v."));
  clearBond();
}

void loop() {
  peripheral.loop();
  addBond();
}

