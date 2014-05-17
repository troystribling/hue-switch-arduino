#include <Wire.h>
#include "utils.h"
#include "peripheral.h"
#include "home_i2c_master.h"

void HomeI2CMaster::begin() {
  Wire.begin();
}

void HomeI2CMaster::writeAndReceiveResponse(uint8_t address, I2CMessage& message, uint8_t requestSize, uint8_t responseSize) {
  uint8_t receivedBytes = 0;
  Wire.beginTransmission(address);
  for(uint8_t i = 0; i < requestSize; i++) {
    Wire.write(message.buffer[i]);
  }
  Wire.endTransmission();
  Wire.requestFrom(address, responseSize);
  while (Wire.available()) {
    message.buffer[receivedBytes] = Wire.read();
    receivedBytes++;
  }
  processResponse(message);
}

void HomeI2CMaster::processResponse(I2CMessage& message) {
    switch (message.messageID) {
      case HUE_LIGHTS_LIGHT_ON_CMD:
        break;
      case HUE_LIGHTS_ALL_LIGHTS_ON_CMD:
        break;
      case HUE_LIGHTS_CREATE_SCENE_CMD:
        break;
      case HUE_LIGHTS_UPDATE_SCENE_CMD:
        break;
      case HUE_LIGHTS_REMOVE_SCENE_CMD:
        break;
      case HUE_LIGHTS_NEXT_SCENE_CMD:
        break;
      case HUE_LIGHTS_GET_SCENE_NAME_CMD:
        break;
      case HUE_LIGHTS_SET_SCENE_NAME_CMD:
        break;
      case HUE_LIGHTS_GET_SCENE_ID_CMD:
        break;
      case HUE_LIGHTS_GET_SCENE_CMD:
        break;
      case HUE_LIGHTS_GET_CURRENT_SCENE_ID_CMD:
        break;
      case HUE_LIGHTS_SET_CURRENT_SCENE_CMD:
        break;
      case HUE_LIGHTS_GET_LIGHT_COLOR_CMD:
        break;
      case HUE_LIGHTS_SET_LIGHT_COLOR_CMD:
        break;
      case HUE_LIGHTS_GET_LIGHT_COUNT_CMD:
        break;
      case HUE_LIGHTS_SET_LIGHT_COUNT_CMD:
        break;
      case HUE_LIGHTS_GET_SCENE_COUNT_CMD:
        break;
      case HUE_LIGHTS_ERASE_EEPROM_CMD:
        break;
      case WIFI_STATUS_CMD:
        break;
      default:
        ERROR_LOG(F("(HomeI2CMaster::processCommand) Command ID is invalid:"));
        ERROR_LOG(message.messageID, DEC);
        break;
  }
}