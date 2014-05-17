#ifndef _ARDUINO_I2C_SLAVE_H
#define _ARDUINO_I2C_SLAVE_H

#include <Wire.h>

class ArduinoI2CSlave {

public:

  ArduinoI2CSlave(uint8_t address) : address(address){};
  ~ArduinoI2CSlave(){};

  void begin();

private:

  uint8_t address;

private:

};

#endif