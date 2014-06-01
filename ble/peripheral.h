#ifndef _PERIPHERAL_H_
#define _PERIPHERAL_H_

class HomeI2CMaster;

#include "blue_cap_peripheral.h"
#include "eeprom_objects.h"

// defaults
#define DEFAULT_UPDATE_PERIOD                   30000

// eeprom storage objects
#define NUMBER_OF_STATE_OBJECTS_OFFSET          0
#define MAX_NUMBER_OF_STATE_OBJECTS             1

struct StateObject {
  uint8_t wifiStatus;
  uint8_t switchState;
};

class Peripheral : public BlueCapBondedPeripheral {

public:

  Peripheral(uint8_t _reqn, uint8_t _rdyn, uint8_t _maxBonds, uint16_t _updatePeriod = DEFAULT_UPDATE_PERIOD);

  void begin();
  void loop();

  void setI2CMaster(HomeI2CMaster* _i2cMaster) {i2cMaster = _i2cMaster;};

  void sendMessageNack(uint8_t messageId);
  void sendSwitchAck(uint8_t* message);

  void setSwitchState(uint8_t switchValue);
  void setWifiStatus(uint8_t wifiStatus);
  void sendLightCount(uint8_t lightCount);
  void sendSceneCount(uint8_t sceneCount);

protected:

  void didReceiveData(uint8_t characteristicId, uint8_t* data, uint8_t size);
  void didReceiveCommandResponse(uint8_t commandId, uint8_t* data, uint8_t size);
  void didDisconnect();
  void didTimeout();
  void didConnect();
  void didStartAdvertising();
  void didReceiveError(uint8_t pipe, uint8_t errorCode);
  void didReceivePipeStatusChange();
  void didBond();
  bool doTimingChange();

private:

  void setLocation(uint8_t* data);
  void setSwitch(uint8_t* data);
  void setSceneName(uint8_t* data, uint8_t size);
  void setCurrentSceneId(uint8_t* data);
  void setLightColor(uint8_t* data);
  void setCommand(uint8_t* data);

  void sendSwitchState(uint8_t switchValue);
  void sendWifiStatus(uint8_t wifiStatus);

  void updateState(StateObject& state);
  void getState(StateObject& state);
  void init();

private:

  uint16_t                      updatePeriod;
  EEPROMObject<StateObject>     stateObjectEEPROM;
  HomeI2CMaster*                i2cMaster;
  bool                          shouldInit;
};

#endif