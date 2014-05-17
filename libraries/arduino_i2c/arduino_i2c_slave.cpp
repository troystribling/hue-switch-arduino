#include "i2c_message.h"
#include "arduino_i2c_slave.h"

I2CMessage requestMessage;
I2CMessage responseMessage;
uint8_t responceSize = 0;

extern "C" {
  void receiveEvent(int _numBytes);
  void requestEvent();
  void procesRequest(uint8_t size);
}

void receiveEvent(int _numBytes) {
  uint8_t bytesReceived = 0;
  while(Wire.available()) {
    requestMessage.buffer[bytesReceived] = Wire.read();
    bytesReceived++;
  }
  procesRequest(bytesReceived);
}

void requestEvent() {
}

void procesRequest(uint8_t size) {
}

void ArduinoI2CSlave::begin() {
  Wire.begin(address);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}
