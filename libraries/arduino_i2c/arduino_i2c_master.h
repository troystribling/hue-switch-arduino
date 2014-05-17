#ifndef _ARDUINO_I2C_MASTER_H_
#define _ARDUINO_I2C_MASTER_H_

#inclue "i2c_message.h"

class ArduinoI2CMaster {

public:

  ArduinoI2CMaster(){};
  ~ArduinoI2CMaster(){};

  begin();
  writeAndReceiveResponse(unit8_t address, I2CMessage& message, uint8_t requestSize, uint8_t responseSize);
  void processResponse(I2CMessage& message, uint8_t size){};

private:

};

#endif