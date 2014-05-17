#ifndef _I2C_MESSAGE_H_
#define _I2C_MESSAGE_H_

#include <Arduino.h>

#define MAX_I2C_MESSAGE_SIZE                    25

struct I2CMessage {
  uint8_t messageID;
  uint8_t buffer[MAX_I2C_MESSAGE_SIZE];
};

#define HUE_LIGHTS_LIGHT_ON_CMD                 1
#define HUE_LIGHTS_ALL_LIGHTS_ON_CMD            2
#define HUE_LIGHTS_CREATE_SCENE_CMD             3
#define HUE_LIGHTS_UPDATE_SCENE_CMD             4
#define HUE_LIGHTS_REMOVE_SCENE_CMD             5
#define HUE_LIGHTS_NEXT_SCENE_CMD               6
#define HUE_LIGHTS_GET_SCENE_NAME_CMD           7
#define HUE_LIGHTS_SET_SCENE_NAME_CMD           8
#define HUE_LIGHTS_GET_SCENE_ID_CMD             9
#define HUE_LIGHTS_GET_SCENE_CMD                10
#define HUE_LIGHTS_GET_CURRENT_SCENE_ID_CMD     11
#define HUE_LIGHTS_SET_CURRENT_SCENE_CMD        12
#define HUE_LIGHTS_GET_LIGHT_COLOR_CMD          13
#define HUE_LIGHTS_SET_LIGHT_COLOR_CMD          14
#define HUE_LIGHTS_GET_LIGHT_COUNT_CMD          15
#define HUE_LIGHTS_SET_LIGHT_COUNT_CMD          16
#define HUE_LIGHTS_GET_SCENE_COUNT_CMD          17
#define HUE_LIGHTS_ERASE_EEPROM_CMD             18
#define WIFI_STATUS_CMD                         19

#endif