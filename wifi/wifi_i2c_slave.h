#ifndef _ARDUINO_I2C_SLAVE_H
#define _ARDUINO_I2C_SLAVE_H

class WifiI2CSlave;

#include "i2c_message.h"

class WifiI2CSlave {

public:

  WifiI2CSlave(HueLightsClient* a_client, uint8_t address);
  ~WifiI2CSlave(){};

  void begin();
  uint8_t* messageBuffer(){return (uint8_t*)&responseMessage;};
  uint8_t messageBufferSize(){return responseMessageSize;};
  void procesRequest(I2CMessage& requestMessage);

private:

  uint8_t           address;
  HueLightsClient*  client;
  HueLightsScene    scene;
  uint8_t           sceneID;
  I2CMessage        responseMessage;
  uint8_t           responseMessageSize;


private:

  void processStatus(I2CMessage& requestMessage);
  void processSetLightOn(I2CMessage& requestMessage);
  void processSetAllLightsOn(I2CMessage& requestMessage);
  void processSetLightColor(I2CMessage& requestMessage);
  void processGetLightColor(I2CMessage& requestMessage);
  void processGetScene(I2CMessage& requestMessage);
  void processSetSceneName(I2CMessage& requestMessage);
  void processSetCurrentScene(I2CMessage& requestMessage);
  void processEraseEEPROM(I2CMessage& requestMessage);
  void processCommand(I2CMessage& requestMessage);

};

#endif