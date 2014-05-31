#include <Wire.h>
#include "utils.h"
#include "peripheral.h"
#include "home_i2c_master.h"
#include "services.h"

#define I2C_TIMEOUT_DELAY   1000
#define I2C_TIMEOUT         15

void HomeI2CMaster::begin() {
  Wire.begin();
}

void HomeI2CMaster::writeAndRead(uint8_t address, I2CMessage& message, size_t requestSize, size_t responseSize) {
  uint8_t transmissionCode = write(address, message, requestSize);
  DBUG_LOG(F("transmissionCode"));
  DBUG_LOG(transmissionCode);
  if (transmissionCode == 0) {
    read(address, responseSize, message.messageID);
  } else {
    ERROR_LOG(F("I2C transmission failed with code"));
    ERROR_LOG(transmissionCode);
  }
}

uint8_t HomeI2CMaster::write(uint8_t address, I2CMessage& message, size_t messageSize) {
  uint8_t bytesReceived = 0;
  DBUG_LOG(F("Begin transmission of message"));
  DBUG_LOG(message.messageID);
  DBUG_LOG(F("data"));
  for (int i = 0; i < messageSize-1; i++) {
    DBUG_LOG(message.buffer[i]);
  }
  Wire.beginTransmission(address);
  Wire.write((uint8_t*)&message, messageSize);
  return Wire.endTransmission();
}

void HomeI2CMaster::read(uint8_t address, size_t messageSize, uint8_t messageID) {
  uint8_t bytesReceived;
  uint8_t retryCount = 0;
  bool retry = true;
  bool readSuccesful = true;
  I2CMessage response;
  DBUG_LOG(F("Receiving response: size, messageID"));
  DBUG_LOG(messageSize);
  DBUG_LOG(messageID);
  while (retry) {
    bytesReceived = 0;
    DBUG_LOG(F("Request Attempt:"));
    DBUG_LOG(retryCount+1);
    Wire.requestFrom(address, messageSize);
    DBUG_LOG(F("Response received"));
    while (Wire.available()) {
      if (bytesReceived == 0) {
        response.messageID = Wire.read();
        if (response.messageID != messageID) {
          DBUG_LOG(F("mesageID invalid. retrying"));
          DBUG_LOG(response.messageID);
          retryCount++;
          if (retryCount < I2C_TIMEOUT) {
            delay(I2C_TIMEOUT_DELAY);
          } else {
            ERROR_LOG(F("I2C Timeout"));
            readSuccesful = false;
            retry = false;
          }
        } else {
          DBUG_LOG(F("messageID valid:"));
          DBUG_LOG(response.messageID);
          retry = false;
        }
      } else {
        if (bytesReceived <= MAX_I2C_MESSAGE_SIZE) {
          response.buffer[bytesReceived-1] = Wire.read();
          DBUG_LOG(F("Data received:"));
          DBUG_LOG(response.buffer[bytesReceived-1], HEX);
        }
      }
      bytesReceived++;
    }
  }
  if (readSuccesful) {
    processResponse(response);
  } else {
    peripheral->sendMessageNack(messageID);
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

void HomeI2CMaster::wifiStatus() {
  DBUG_LOG(F("wifiStatus"));
  I2CMessage message;
  message.messageID = WIFI_STATUS_CMD;
  writeAndRead(HUE_LIGHTS_I2C_ADDRESS, message, WIFI_STATUS_CMD_REQUEST_SIZE, WIFI_STATUS_CMD_RESPONSE_SIZE);
}

void HomeI2CMaster::setSwitch(uint8_t value) {
  DBUG_LOG(F("setSwitch: value"));
  DBUG_LOG(value);
  I2CMessage message;
  message.messageID = HUE_LIGHTS_ALL_LIGHTS_ON_CMD;
  message.buffer[0] = value;
  writeAndRead(HUE_LIGHTS_I2C_ADDRESS, message, HUE_LIGHTS_ALL_LIGHTS_ON_CMD_REQUEST_SIZE, HUE_LIGHTS_ALL_LIGHTS_ON_CMD_RESPONSE_SIZE);
}

void HomeI2CMaster::numberOfHueLights() {
  DBUG_LOG(F("numberOfHueLights"));
  I2CMessage message;
  message.messageID = HUE_LIGHTS_GET_LIGHT_COUNT_CMD;
  writeAndRead(HUE_LIGHTS_I2C_ADDRESS, message, HUE_LIGHTS_GET_LIGHT_COUNT_CMD_REQUEST_SIZE, HUE_LIGHTS_GET_LIGHT_COUNT_CMD_RESPONSE_SIZE);
}

void HomeI2CMaster::numberOfScenes() {
  DBUG_LOG(F("numberOfScenes"));
  I2CMessage message;
  message.messageID = HUE_LIGHTS_GET_SCENE_COUNT_CMD;
  writeAndRead(HUE_LIGHTS_I2C_ADDRESS, message, HUE_LIGHTS_GET_SCENE_COUNT_CMD_REQUEST_SIZE, HUE_LIGHTS_GET_SCENE_COUNT_CMD_RESPONSE_SIZE);
}
