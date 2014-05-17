#ifndef _HOME_I2C_MASTER_H_
#define _HOME_I2C_MASTER_H_

class Peripheral;

#include "i2c_message.h"

class HomeI2CMaster {

public:

  HomeI2CMaster(Peripheral* a_peripheral) : peripheral(a_peripheral) {};
  ~HomeI2CMaster(){};

  void begin();
  void writeAndReceiveResponse(uint8_t address, I2CMessage& message, uint8_t requestSize, uint8_t responseSize);
  void processResponse(I2CMessage& message);

private:

  Peripheral* peripheral;

};

#endif