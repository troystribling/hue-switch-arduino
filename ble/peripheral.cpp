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
    DBUG_LOG(updatePeriod);
    i2cMaster->wifiStatus();
  }
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

// update attributes
void Peripheral::setLocation(uint8_t* data, uint8_t size) {
}

void Peripheral::setSwitch(uint8_t* data, uint8_t size) {
  uint8_t switchState = data[0];
  if (sendAck(PIPE_HUE_LIGHTS_HUE_SWITCH_RX_ACK)) {
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

// state
void Peripheral::setSwitchState(uint8_t switchState) {
  StateObject state;
  getState(state);
  if (state.switchState != switchState) {
    state.switchState = switchState;
    sendData(PIPE_HUE_LIGHTS_HUE_SWITCH_TX, &state.wifiStatus, PIPE_HUE_LIGHTS_HUE_SWITCH_TX_MAX_SIZE);
    setData(PIPE_HUE_LIGHTS_HUE_SWITCH_SET, &state.switchState, PIPE_HUE_LIGHTS_HUE_SWITCH_SET_MAX_SIZE);
    updateState(state);
  }
}

void Peripheral::setWifiStatusState(uint8_t wifiStatus) {
  StateObject state;
  getState(state);
  if (state.wifiStatus != wifiStatus) {
    state.wifiStatus = wifiStatus;
    sendData(PIPE_HUE_LIGHTS_HUE_STATUS_TX, &state.wifiStatus, PIPE_HUE_LIGHTS_HUE_STATUS_TX_MAX_SIZE);
    setData(PIPE_HUE_LIGHTS_HUE_STATUS_SET, &state.wifiStatus, PIPE_HUE_LIGHTS_HUE_STATUS_SET_MAX_SIZE);
    updateState(state);
  }
}

void Peripheral::updateState(StateObject& state) {
  DBUG_LOG(F("Peripheral::setState"));
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
  setSwitchState(state.switchState);
  setWifiStatusState(state.wifiStatus);
  i2cMaster->allLightsOn(state.switchState);
}

