#ifndef _ARDUINO_I2C_SLAVE_H
#define _ARDUINO_I2C_SLAVE_H

class WifiI2CSlave;

#include "i2c_message.h"

class WifiI2CSlave {

public:

  WifiI2CSlave(HueLightsClient* a_client, uint8_t address);
  ~WifiI2CSlave(){};

  void begin();
  void listen();
  void processRequest();

private:

  uint8_t           address;
  HueLightsClient*  client;
  HueLightsScene    scene;
  uint8_t           sceneID;

private:

  void processStatus(I2CMessage& requestMessage);
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