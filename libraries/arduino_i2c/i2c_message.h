#ifndef _I2C_MESSAGE_H_
#define _I2C_MESSAGE_H_

#define MAX_MESSAGE_SIZE    25

struct I2CMessage {
  uint8_t messageId;
  uint8_t buffer[MAX_MESSAGE_SIZE];
};

#endif