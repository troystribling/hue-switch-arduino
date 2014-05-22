#include <Wire.h>
#include "utils.h"
#include "peripheral.h"
#include "home_i2c_master.h"
#include "services.h"

void HomeI2CMaster::begin() {
  Wire.begin();
}

void HomeI2CMaster::writeAndReceiveResponse(uint8_t address, I2CMessage& message, size_t requestSize, size_t responseSize) {
  uint8_t bytesReceived = 0;
  DBUG_LOG(F("Begin transmission of message"));
  DBUG_LOG(message.messageID);
  Wire.beginTransmission(address);
  Wire.write((uint8_t*)&message, responseSize);
  Wire.endTransmission();
  DBUG_LOG(F("Receiving response of size"));
  DBUG_LOG(responseSize);
  Wire.requestFrom(address, responseSize);
  while (Wire.available()) {
    if (bytesReceived == 0) {
      message.messageID = Wire.read();
      DBUG_LOG(F("Message ID"));
      DBUG_LOG(message.messageID);
    } else {
      if (bytesReceived <= MAX_I2C_MESSAGE_SIZE) {
        message.buffer[bytesReceived-1] = Wire.read();
        DBUG_LOG(F("Data received: byte, val"));
        DBUG_LOG(bytesReceived-1);
        DBUG_LOG(message.buffer[bytesReceived-1], HEX);
      }
    }
    bytesReceived++;
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
  peripheral->setWifiStatus(message.buffer[0]);
}
