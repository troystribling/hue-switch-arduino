#include <Wire.h>
#include "utils.h"
#include "peripheral.h"
#include "home_i2c_master.h"

void HomeI2CMaster::begin() {
  Wire.begin();
}

void HomeI2CMaster::writeAndReceiveResponse(uint8_t address, I2CMessage& message, uint8_t requestSize, uint8_t responseSize) {
  uint8_t receivedBytes = 0;
  DBUG_LOG(F("Begin transmission of message"));
  DBUG_LOG(message.messageID);
  Wire.beginTransmission(address);
  Wire.write(message.messageID);
  for(uint8_t i = 0; i < requestSize; i++) {
    Wire.write(message.buffer[i]);
  }
  Wire.endTransmission();
  Wire.requestFrom(address, responseSize);
  DBUG_LOG(F("Receiving response"));
  while (Wire.available()) {
    if (receivedBytes == 0) {
      message.messageID = Wire.read();
      DBUG_LOG(F("Message ID"));
      DBUG_LOG(message.messageID);
    } else {
      message.buffer[receivedBytes] = Wire.read();
      DBUG_LOG(F("Data received"));
      DBUG_LOG(message.buffer[receivedBytes]);
      receivedBytes++;
      DBUG_LOG(F("Received bytes"));
      DBUG_LOG(receivedBytes);
    }
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
        DBUG_LOG(F("WIFI_STATUS_CMD"));
        wifiStatusResponse(message);
        break;
      default:
        ERROR_LOG(F("(HomeI2CMaster::processCommand) Command ID is invalid:"));
        ERROR_LOG(message.messageID, DEC);
        break;
  }
}

void  HomeI2CMaster::wifiStatus() {
  I2CMessage message;
  message.messageID = WIFI_STATUS_CMD;
  writeAndReceiveResponse(HUE_LIGHTS_I2C_ADDRESS, message, WIFI_STATUS_REQUEST_SIZE, WIFI_STATUS_RESPONSE_SIZE);
}

void HomeI2CMaster::wifiStatusResponse(I2CMessage& message) {
}
