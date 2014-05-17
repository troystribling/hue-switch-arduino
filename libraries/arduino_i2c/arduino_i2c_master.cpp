#include <Wire.h>
#include "arduino_i2c_master.h"

ArduinoI2CMaster::begin() {
  Wire.begin();
}

ArduinoI2CMaster::writeAndReceiveResponse(unit8_t address, I2CMessage& message, uint8_t requestSize, uint8_t responseSize) {
  uint8_t receivedBytes = 0;
  Wire.beginTransmission(address):
  for(uint8_t i = 0; i < requestSize; i++) {
    Wire.write(message.buffer[i]);
  }
  Wire.endTransmission();
  Wire.requestFrom(address, responseSize);
  while (Wire.available()) {
    message.buffer[receivedBytes] = Wire.read();
    receivedBytes++;
  }
  processResponse(message, receivedBytes);
}
