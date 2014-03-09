#include "utils.h"
#include "blue_cap_peripheral.h"
#include "peripheral.h"
#include "services.h"

static services_pipe_type_mapping_t services_pipe_type_mapping[NUMBER_OF_PIPES] = SERVICES_PIPE_TYPE_MAPPING_CONTENT;
static hal_aci_data_t setup_msgs[NB_SETUP_MESSAGES] PROGMEM = SETUP_MESSAGES_CONTENT;

Peripheral::Peripheral(uint8_t _reqn, uint8_t _rdyn, uint16_t _eepromOffset, uint8_t _maxBonds) : BlueCapBondedPeripheral(_reqn, _rdyn, _eepromOffset, _maxBonds) {
  setServicePipeTypeMapping(services_pipe_type_mapping, NUMBER_OF_PIPES);
  setSetUpMessages(setup_msgs, NB_SETUP_MESSAGES);
}

void Peripheral::begin() {
  INFO_LOG(F("Peripheral::begin"));
  BlueCapBondedPeripheral::begin();
}

void Peripheral::loop() {
  BlueCapBondedPeripheral::loop();
}

void Peripheral::didReceiveData(uint8_t characteristicId, uint8_t* data, uint8_t size) {
  switch(characteristicId) {
    case PIPE_LOCATION_LATITUDE_AND_LONGITUDE_RX_ACK:
      break;
    case PIPE_HUE_LIGHTS_HUE_SWITCHES_RX_ACK:
      break;
    case PIPE_HUE_LIGHTS_HUE_SCENE_NAME_RX_ACK:
      break;
    case PIPE_HUE_LIGHTS_CURRENT_HUE_SCENE_ID_RX_ACK:
      break;
    case PIPE_HUE_LIGHTS_HUE_LIGHT_COLOR_RX_ACK:
      break;
    case PIPE_HUE_LIGHTS_HUE_LIGHTS_COMMAND_RX_ACK:
      break;
    default:
      break;
  }
}

void Peripheral::didDisconnect() {
}

void Peripheral::didTimeout() {
}

void Peripheral::didConnect() {
}

void Peripheral::didStartAdvertising() {
}

void Peripheral::didReceiveError(uint8_t pipe, uint8_t errorCode) {
}

void Peripheral::didReceivePipeStatusChange() {
}

void Peripheral::didBond() {
}

bool Peripheral::doTimingChange() {
}
