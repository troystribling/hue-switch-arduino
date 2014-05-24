#include <Wire.h>
#include "utils.h"
#include "hue_lights_client.h"
#include "wifi_i2c_slave.h"

static WifiI2CSlave* i2cSlave = NULL;

extern "C" {
  void receiveEvent(int numBytes);
  void requestEvent();
}

void receiveEvent(int numBytes) {
  I2CMessage message;
  uint8_t bytesReceived = 0;
  while(Wire.available()) {
    if (bytesReceived == 0) {
      message.messageID = Wire.read();
    } else {
      if (bytesReceived <= MAX_I2C_MESSAGE_SIZE) {
        message.buffer[bytesReceived-1] = Wire.read();
      }
    }
    bytesReceived++;
  }
  i2cSlave->procesRequest(message);
}

void requestEvent() {
  DBUG_LOG(F("request message size, buffer"));
  DBUG_LOG(i2cSlave->messageBufferSize());
  for (int i = 0; i < i2cSlave->messageBufferSize(); i++) {
    DBUG_LOG(i2cSlave->messageBuffer()[i]);
  }
  Wire.write(i2cSlave->messageBuffer(), i2cSlave->messageBufferSize());
}

WifiI2CSlave::WifiI2CSlave(HueLightsClient* a_client, uint8_t address) : address(address), client(a_client) {
  i2cSlave = this;
}

void WifiI2CSlave::begin() {
  Wire.begin(address);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

void  WifiI2CSlave::procesRequest(I2CMessage& requestMessage) {
  DBUG_FREE_MEMORY;
  switch (requestMessage.messageID) {
      case HUE_LIGHTS_ALL_LIGHTS_ON_CMD:
        DBUG_LOG(F("HUE_LIGHTS_ALL_LIGHTS_ON_CMD"));
        processSetAllLightsOn(requestMessage);
        break;
      case HUE_LIGHTS_CREATE_SCENE_CMD:
        scene.status = 0x01;
        sceneID = client->createScene(scene);
        break;
      case HUE_LIGHTS_UPDATE_SCENE_CMD:
        client->updateScene(sceneID, scene);
        break;
      case HUE_LIGHTS_REMOVE_SCENE_CMD:
        client->removeScene(sceneID);
        break;
      case HUE_LIGHTS_NEXT_SCENE_CMD:
        client->nextScene(sceneID, scene);
        break;
      case HUE_LIGHTS_GET_SCENE_NAME_CMD:
        INFO_LOG(scene.name);
        break;
      case HUE_LIGHTS_SET_SCENE_NAME_CMD:
        processSetSceneName(requestMessage);
        break;
      case HUE_LIGHTS_GET_SCENE_ID_CMD:
        INFO_LOG(sceneID);
        break;
      case HUE_LIGHTS_GET_SCENE_CMD:
        processGetScene(requestMessage);
        break;
      case HUE_LIGHTS_GET_CURRENT_SCENE_ID_CMD:
        INFO_LOG(client->getCurrentSceneID());
        break;
      case HUE_LIGHTS_SET_CURRENT_SCENE_CMD:
        processSetCurrentScene(requestMessage);
        break;
      case HUE_LIGHTS_GET_LIGHT_COLOR_CMD:
        processGetLightColor(requestMessage);
        break;
      case HUE_LIGHTS_SET_LIGHT_COLOR_CMD:
        processSetLightColor(requestMessage);
        break;
      case HUE_LIGHTS_GET_LIGHT_COUNT_CMD:
        INFO_LOG(client->getLightCount());
        break;
      case HUE_LIGHTS_SET_LIGHT_COUNT_CMD:
        client->setLightCount();
        break;
      case HUE_LIGHTS_GET_SCENE_COUNT_CMD:
        INFO_LOG(client->getSceneCount());
        break;
      case HUE_LIGHTS_ERASE_EEPROM_CMD:
        processEraseEEPROM(requestMessage);
        break;
      case WIFI_STATUS_CMD:
        DBUG_LOG(F("WIFI_STATUS_CMD"));
        responseMessage.messageID = WIFI_STATUS_CMD;
        responseMessage.buffer[0] = client->lanConnected();
        responseMessageSize = WIFI_STATUS_RESPONSE_SIZE;
        break;
      default:
        ERROR_LOG(F("I2C Command invalid:"));
        ERROR_LOG(requestMessage.messageID, DEC);
        break;
  }
}

void WifiI2CSlave::processSetAllLightsOn(I2CMessage& requestMessage) {
  bool lightOn = (bool)requestMessage.buffer[0];
  bool status = client->setAllLightsOn(lightOn);
  DBUG_LOG(F("processSetAllLightsOn, lightOn, status"));
  DBUG_LOG(lightOn);
  DBUG_LOG(status);
  responseMessage.messageID = HUE_LIGHTS_ALL_LIGHTS_ON_CMD;
  responseMessage.buffer[0] = status;
  responseMessage.buffer[1] = lightOn;
  responseMessageSize = HUE_LIGHTS_ALL_LIGHTS_ON_CMD_RESPONSE_SIZE;
}

void WifiI2CSlave::processSetLightColor(I2CMessage& requestMessage) {
  HueLight light;
  uint8_t lightID = requestMessage.buffer[0];
  light.brightness = requestMessage.buffer[1];
  light.saturation = requestMessage.buffer[2];
  light.hue = requestMessage.buffer[3];
  if (client->setLightColor(lightID, light)) {
    scene.lights[lightID].brightness = light.brightness;
    scene.lights[lightID].saturation = light.saturation;
    scene.lights[lightID].hue = light.hue;
  }
}

void WifiI2CSlave::processGetLightColor(I2CMessage& requestMessage) {
  uint8_t lightID = requestMessage.buffer[0];
}

void WifiI2CSlave::processGetScene(I2CMessage& requestMessage) {
  sceneID = requestMessage.buffer[0];
  client->getScene(sceneID, scene);
}

void WifiI2CSlave::processSetSceneName(I2CMessage& requestMessage) {
  char* sceneName = (char*)requestMessage.buffer;
  strcpy(scene.name, sceneName);
}

void WifiI2CSlave::processSetCurrentScene(I2CMessage& requestMessage) {
  sceneID = requestMessage.buffer[0];
  client->setCurrentScene(sceneID);
}

void WifiI2CSlave::processEraseEEPROM(I2CMessage& requestMessage) {
  for (int i = 0; i < 1024; i++) {
    EEPROM.write(i, 0x00);
  }
}

