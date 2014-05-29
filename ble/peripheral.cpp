#include "utils.h"
#include "blue_cap_peripheral.h"
#include "home_i2c_master.h"
#include "peripheral.h"
#include "services.h"

#define EEPROM_OFFSET         32

static services_pipe_type_mapping_t services_pipe_type_mapping[NUMBER_OF_PIPES] = SERVICES_PIPE_TYPE_MAPPING_CONTENT;
static hal_aci_data_t setup_msgs[NB_SETUP_MESSAGES] PROGMEM = SETUP_MESSAGES_CONTENT;

Peripheral::Peripheral(uint8_t _reqn, uint8_t _rdyn, uint8_t _maxBonds, uint16_t _updatePeriod) :
  BlueCapBondedPeripheral(_reqn, _rdyn, EEPROM_OFFSET, _maxBonds),
  updatePeriod(_updatePeriod),
  stateObjectEEPROM(EEPROMObject<StateObject>(NUMBER_OF_STATE_OBJECTS_OFFSET, MAX_NUMBER_OF_STATE_OBJECTS)) {
  setServicePipeTypeMapping(services_pipe_type_mapping, NUMBER_OF_PIPES);
  setSetUpMessages(setup_msgs, NB_SETUP_MESSAGES);
}

void Peripheral::begin() {
  DBUG_LOG(F("Peripheral::begin"));
  BlueCapBondedPeripheral::begin();
}

void Peripheral::loop() {
  if (millis() % updatePeriod == 0) {
    i2cMaster->wifiStatus();
  }
  BlueCapBondedPeripheral::loop();
}

// callbacks
void Peripheral::didReceiveData(uint8_t characteristicId, uint8_t* data, uint8_t size) {
  switch(characteristicId) {
    case PIPE_LOCATION_LATITUDE_AND_LONGITUDE_RX_ACK:
      DBUG_LOG(F("PIPE_LOCATION_LATITUDE_AND_LONGITUDE_RX_ACK"));
      setLocation(data);
      break;
    case PIPE_HUE_LIGHTS_HUE_SWITCH_RX_ACK:
      DBUG_LOG(F("PIPE_HUE_LIGHTS_HUE_SWITCH_RX_ACK"));
      setSwitch(data);
      break;
    case PIPE_HUE_LIGHTS_HUE_SCENE_NAME_RX_ACK:
      DBUG_LOG(F("PIPE_HUE_LIGHTS_HUE_SCENE_NAME_RX_ACK"));
      setSceneName(data, size);
      break;
    case PIPE_HUE_LIGHTS_CURRENT_HUE_SCENE_ID_RX_ACK:
      DBUG_LOG(F("PIPE_HUE_LIGHTS_CURRENT_HUE_SCENE_ID_RX_ACK"));
      setCurrentSceneId(data);
      break;
    case PIPE_HUE_LIGHTS_HUE_LIGHT_COLOR_RX_ACK:
      DBUG_LOG(F("PIPE_HUE_LIGHTS_HUE_LIGHT_COLOR_RX_ACK"));
      setLightColor(data);
      break;
    case PIPE_HUE_LIGHTS_HUE_LIGHTS_COMMAND_RX_ACK:
      DBUG_LOG(F("PIPE_HUE_LIGHTS_HUE_LIGHTS_COMMAND_RX_ACK"));
      setCommand(data);
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
  DBUG_LOG(F("didDisconnect"));
}

void Peripheral::didTimeout() {
}

void Peripheral::didConnect() {
  DBUG_LOG(F("didConnect"));
}

void Peripheral::didStartAdvertising() {
    initState();
}

void Peripheral::didReceiveError(uint8_t pipe, uint8_t errorCode) {
}

void Peripheral::didReceivePipeStatusChange() {
}

void Peripheral::didBond() {
}

bool Peripheral::doTimingChange() {
}

// acks
void Peripheral::sendMessageNack(uint8_t messageID) {
  switch (messageID) {
    case HUE_LIGHTS_ALL_LIGHTS_ON_CMD:
      sendNack(PIPE_HUE_LIGHTS_HUE_SWITCH_RX_ACK, HUE_LIGHTS_ERROR);
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
    default:
      ERROR_LOG(F("(Peripheral::sendNack) Command ID is invalid:"));
      ERROR_LOG(messageID, DEC);
      break;
  }
}

void Peripheral::sendSwitchAck(uint8_t* data) {
  DBUG_LOG(F("sendSwitchAck: value"));
  DBUG_LOG(data[0]);
  setSwitchState(data[0]);
}

// update attributes
void Peripheral::setLocation(uint8_t* data) {
}

void Peripheral::setSwitch(uint8_t* data) {
  uint8_t switchState = data[0];
  DBUG_LOG(F("setSwitch: switchState"));
  DBUG_LOG(switchState);
  i2cMaster->setSwitch(switchState);
}

void Peripheral::setSceneName(uint8_t* data, uint8_t size) {
}

void Peripheral::setCurrentSceneId(uint8_t* data) {
}

void Peripheral::setLightColor(uint8_t* data) {
}

void Peripheral::setCommand(uint8_t* data) {
}

// state
void Peripheral::setSwitchState(uint8_t switchState) {
  StateObject state;
  getState(state);
  if (state.switchState != switchState) {
    state.switchState = switchState;
    sendSwitchState(state.switchState);
    updateState(state);
  }
}

void Peripheral::setWifiStatus(uint8_t wifiStatus) {
  StateObject state;
  getState(state);
  if (state.wifiStatus != wifiStatus) {
    state.wifiStatus = wifiStatus;
    sendWifiStatus(state.wifiStatus);
    updateState(state);
  }
}

void Peripheral::updateState(StateObject& state) {
  DBUG_LOG(F("Peripheral::updateState: wifiStatus, switchState"));
  DBUG_LOG(state.wifiStatus);
  DBUG_LOG(state.switchState);
  stateObjectEEPROM.update(0, state);
}

void Peripheral::getState(StateObject& state) {
  stateObjectEEPROM.read(0, state);
}

void Peripheral::initState() {
  StateObject state;
  getState(state);
  DBUG_LOG(F("initState: wifiStatus, switchStatus"));
  DBUG_LOG(state.wifiStatus);
  DBUG_LOG(state.switchState);
  sendWifiStatus(state.wifiStatus);
  sendSwitchState(state.switchState);
  i2cMaster->setSwitch(state.switchState);
}

void Peripheral::sendSwitchState(uint8_t switchState) {
  DBUG_LOG(F("sendSwitchState"));
  DBUG_LOG(switchState);
  sendData(PIPE_HUE_LIGHTS_HUE_SWITCH_TX, &switchState, PIPE_HUE_LIGHTS_HUE_SWITCH_TX_MAX_SIZE);
  setData(PIPE_HUE_LIGHTS_HUE_SWITCH_SET, &switchState, PIPE_HUE_LIGHTS_HUE_SWITCH_SET_MAX_SIZE);
}

void Peripheral::sendWifiStatus(uint8_t wifiStatus) {
  DBUG_LOG(F("sendWifiStatus"));
  DBUG_LOG(wifiStatus);
  sendData(PIPE_HUE_LIGHTS_HUE_STATUS_TX, &wifiStatus, PIPE_HUE_LIGHTS_HUE_STATUS_TX_MAX_SIZE);
  setData(PIPE_HUE_LIGHTS_HUE_STATUS_SET, &wifiStatus, PIPE_HUE_LIGHTS_HUE_STATUS_SET_MAX_SIZE);
}

