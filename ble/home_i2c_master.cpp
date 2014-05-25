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
  bool errorStatus = true;
  DBUG_LOG(F("Begin transmission of message"));
  DBUG_LOG(message.messageID);
  Wire.beginTransmission(address);
  Wire.write((uint8_t*)&message, responseSize);
  uint8_t transmissionCode = Wire.endTransmission();
  DBUG_LOG(F("transmissionCode"));
  DBUG_LOG(transmissionCode);
  if (transmissionCode == 0) {
    DBUG_LOG(F("Receiving response of size"));
    DBUG_LOG(responseSize);
    Wire.requestFrom(address, responseSize);
    DBUG_LOG(F("Response received"));
    while (Wire.available()) {
      if (bytesReceived == 0) {
        uint8_t messageID = Wire.read();
        if (messageID == message.messageID) {
          message.messageID = messageID;
          DBUG_LOG(F("Message ID:"));
          DBUG_LOG(message.messageID);
        } else {
          ERROR_LOG(F("Response messageID invalid:"));
          ERROR_LOG(messageID);
          errorStatus = false;
        }
      } else {
        if (bytesReceived <= MAX_I2C_MESSAGE_SIZE) {
          message.buffer[bytesReceived-1] = Wire.read();
          DBUG_LOG(F("Data received:"));
          DBUG_LOG(message.buffer[bytesReceived-1], HEX);
        }
      }
      bytesReceived++;
    }
    if (errorStatus) {
      processResponse(message);
    }
  } else {
    ERROR_LOG(F("I2C transmission failed with code"));
    ERROR_LOG(transmissionCode);
  }
}

void HomeI2CMaster::processResponse(I2CMessage& message) {
    switch (message.messageID) {
      case HUE_LIGHTS_ALL_LIGHTS_ON_CMD:
        DBUG_LOG(F("HUE_LIGHTS_ALL_LIGHTS_ON_CMD"));
        peripheral->sendSwitchAck(message.buffer);
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
        peripheral->setWifiStatus(message.buffer[0]);
        break;
      default:
        ERROR_LOG(F("(HomeI2CMaster::processCommand) Command ID is invalid:"));
        ERROR_LOG(message.messageID, DEC);
        break;
  }
}

void  HomeI2CMaster::wifiStatus() {
  DBUG_LOG(F("wifiStatus"));
  I2CMessage message;
  message.messageID = WIFI_STATUS_CMD;
  writeAndReceiveResponse(HUE_LIGHTS_I2C_ADDRESS, message, WIFI_STATUS_REQUEST_SIZE, WIFI_STATUS_RESPONSE_SIZE);
}

void HomeI2CMaster::setSwitch(uint8_t value) {
  DBUG_LOG(F("setSwitch: value"));
  DBUG_LOG(value);
  I2CMessage message;
  message.messageID = HUE_LIGHTS_ALL_LIGHTS_ON_CMD;
  message.buffer[0] = value;
  writeAndReceiveResponse(HUE_LIGHTS_I2C_ADDRESS, message, HUE_LIGHTS_ALL_LIGHTS_ON_CMD_REQUEST_SIZE, HUE_LIGHTS_ALL_LIGHTS_ON_CMD_RESPONSE_SIZE);
}
