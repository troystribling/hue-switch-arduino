#ifndef _ARDUINO_I2C_SLAVE_H
#define _ARDUINO_I2C_SLAVE_H

#include <Wire.h>
#include "hue_lights_client.h"
#include "i2c_message.h"

class WifiI2CSlave {

public:

  WifiI2CSlave(HueLightsClient* a_client, uint8_t address) : address(address), client(a_client);
  ~WifiI2CSlave(){};

  void begin();
  uint8_t* messageBuffer(){return (uint8_t*)responseMessageBuffer};
  uint8_t messageBufferSize(){return responseMessageSize};

private:

  uint8_t           address;
  HueLightsClient*  client;
  HueLightsScene    scene;
  uint8_t           sceneID;
  I2CMessage        responseMessageBuffer;
  uint8_t           responseMessageSize;


private:

  void procesRequest(I2CMessage& requestMessage, uint8_t requestSize);
  void processStatus(I2CMessage& requestMessage, uint8_t requestSize);
  void processSetLightOn(I2CMessage& requestMessage, uint8_t requestSize);
  void processSetAllLightsOn(I2CMessage& requestMessage, uint8_t requestSize);
  void processSetLightColor(I2CMessage& requestMessage, uint8_t requestSize);
  void processGetLightColor(I2CMessage& requestMessage, uint8_t requestSize);
  void processGetScene(I2CMessage& requestMessage, uint8_t requestSize);
  void processSetSceneName(I2CMessage& requestMessage, uint8_t requestSize);
  void processSetCurrentScene(I2CMessage& requestMessage, uint8_t requestSize);
  void processEraseEEPROM(I2CMessage& requestMessage, uint8_t requestSize);
  void processCommand(I2CMessage& requestMessage, uint8_t requestSize);

};

#endif