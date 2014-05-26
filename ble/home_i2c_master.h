#ifndef _HOME_I2C_MASTER_H_
#define _HOME_I2C_MASTER_H_

class Peripheral;

#include "i2c_message.h"

class HomeI2CMaster {

public:

  HomeI2CMaster(Peripheral* a_peripheral) : peripheral(a_peripheral) {};
  ~HomeI2CMaster(){};

  void begin();
  void writeAndRead(uint8_t address, I2CMessage& message, size_t requestSize, size_t responseSize);
  uint8_t write(uint8_t address, I2CMessage& message, size_t messageSize);
  void read(uint8_t address, size_t messageSize, uint8_t messageID);
  void processResponse(I2CMessage& message);

  void wifiStatus();
  void setSwitch(uint8_t value);

private:

  Peripheral* peripheral;

};

#endif