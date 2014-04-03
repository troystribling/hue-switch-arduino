#include "utils.h"
#include "blue_cap_peripheral.h"
#include "peripheral.h"
#include "services.h"

#define EEPROM_OFFSET         32

static services_pipe_type_mapping_t services_pipe_type_mapping[NUMBER_OF_PIPES] = SERVICES_PIPE_TYPE_MAPPING_CONTENT;
static hal_aci_data_t setup_msgs[NB_SETUP_MESSAGES] PROGMEM = SETUP_MESSAGES_CONTENT;

Peripheral::Peripheral(uint8_t _reqn, uint8_t _rdyn, uint8_t _maxBonds) :
  BlueCapBondedPeripheral(_reqn, _rdyn, EEPROM_OFFSET, _maxBonds),
  stateObjectEEPROM(EEPROMObject<StateObject>(NUMBER_OF_STATE_OBJECTS_OFFSET, MAX_NUMBER_OF_STATE_OBJECTS)) {
  setServicePipeTypeMapping(services_pipe_type_mapping, NUMBER_OF_PIPES);
  setSetUpMessages(setup_msgs, NB_SETUP_MESSAGES);
}

void Peripheral::begin() {
  DBUG_LOG(F("Peripheral::begin"));
  BlueCapBondedPeripheral::begin();
  setState();
}

void Peripheral::loop() {
  BlueCapBondedPeripheral::loop();
}

// callbacks
void Peripheral::didReceiveData(uint8_t characteristicId, uint8_t* data, uint8_t size) {
  switch(characteristicId) {
    case PIPE_LOCATION_LATITUDE_AND_LONGITUDE_RX_ACK:
      DBUG_LOG(F("PIPE_LOCATION_LATITUDE_AND_LONGITUDE_RX_ACK"));
      setLocation(data, size);
      break;
    case PIPE_HUE_LIGHTS_HUE_SWITCH_RX_ACK:
      DBUG_LOG(F("PIPE_HUE_LIGHTS_HUE_SWITCH_RX_ACK"));
      setSwitch(data, size);
      break;
    case PIPE_HUE_LIGHTS_HUE_SCENE_NAME_RX_ACK:
      DBUG_LOG(F("PIPE_HUE_LIGHTS_HUE_SCENE_NAME_RX_ACK"));
      setSceneName(data, size);
      break;
    case PIPE_HUE_LIGHTS_CURRENT_HUE_SCENE_ID_RX_ACK:
      DBUG_LOG(F("PIPE_HUE_LIGHTS_CURRENT_HUE_SCENE_ID_RX_ACK"));
      setCurrentSceneId(data, size);
      break;
    case PIPE_HUE_LIGHTS_HUE_LIGHT_COLOR_RX_ACK:
      DBUG_LOG(F("PIPE_HUE_LIGHTS_HUE_LIGHT_COLOR_RX_ACK"));
      setLightColor(data, size);
      break;
    case PIPE_HUE_LIGHTS_HUE_LIGHTS_COMMAND_RX_ACK:
      DBUG_LOG(F("PIPE_HUE_LIGHTS_HUE_LIGHTS_COMMAND_RX_ACK"));
      setCommand(data, size);
      break;
    default:
      break;
  }
}

void Peripheral::didReceiveCommandResponse(uint8_t commandId, uint8_t* data, uint8_t size) {
  switch(commandId) {
    case ACI_CMD_CONNECT:
      DBUG_LOG(F("ACI_CMD_CONNECT response received"));
      break;
    case ACI_CMD_GET_DEVICE_VERSION:
      DBUG_LOG(F("ACI_CMD_GET_DEVICE_VERSION response received"));
      break;
    case  ACI_CMD_SET_LOCAL_DATA:
      DBUG_LOG(F("ACI_CMD_SET_LOCAL_DATA response received"));
      break;
    case ACI_CMD_SEND_DATA:
      DBUG_LOG(F("ACI_CMD_SEND_DATA response received"));
      break;
    case ACI_CMD_SEND_DATA_ACK:
      DBUG_LOG(F("ACI_CMD_SEND_DATA_ACK response received"));
      break;
    case ACI_CMD_SEND_DATA_NACK:
      DBUG_LOG(F("ACI_CMD_SEND_DATA_NACK response received"));
      break;
    case ACI_CMD_CHANGE_TIMING:
      DBUG_LOG(F("ACI_CMD_CHANGE_TIMING response received"));
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

// update attributes
void Peripheral::setLocation(uint8_t* data, uint8_t size) {
}

void Peripheral::setSwitch(uint8_t* data, uint8_t size) {
  if (sendAck(PIPE_HUE_LIGHTS_HUE_SWITCH_RX_ACK)) {
    StateObject stateObject;
    stateObjectEEPROM.read(0, stateObject);
    memcpy(&stateObject.switchValue, data, 1);
    stateObjectEEPROM.update(0, stateObject);
    DBUG_LOG(F("Peripheral::setSwitch::switchValue:"));
    DBUG_LOG(stateObject.switchValue);
  }
}

void Peripheral::setSceneName(uint8_t* data, uint8_t size) {
}

void Peripheral::setCurrentSceneId(uint8_t* data, uint8_t size) {
}

void Peripheral::setLightColor(uint8_t* data, uint8_t size) {
}

void Peripheral::setCommand(uint8_t* data, uint8_t size) {
}

// setup
void Peripheral::setState() {
  StateObject stateObject;
  stateObjectEEPROM.read(0, stateObject);
  sendData(PIPE_HUE_LIGHTS_HUE_SWITCH_TX, &stateObject.switchValue, PIPE_HUE_LIGHTS_HUE_SWITCH_TX_MAX_SIZE);
  setData(PIPE_HUE_LIGHTS_HUE_SWITCH_SET, &stateObject.switchValue, PIPE_HUE_LIGHTS_HUE_SWITCH_SET_MAX_SIZE);
}
