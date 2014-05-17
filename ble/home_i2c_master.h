#ifndef _HOME_I2C_MASTER_H_
#define _HOME_I2C_MASTER_H_

#inclue "i2c_message.h"

class HomeI2CMaster {

public:

  HomeI2CMaster(){};
  ~HomeI2CMaster(){};

  begin();
  writeAndReceiveResponse(unit8_t address, I2CMessage& message, uint8_t requestSize, uint8_t responseSize);
  void processResponse(I2CMessage& message, uint8_t size){};

private:

};

#endif