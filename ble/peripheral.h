#ifndef _PERIPHERAL_H_
#define _PERIPHERAL_H_

#include "blue_cap_peripheral.h"

class Peripheral : public BlueCapBondedPeripheral {

public:

  Peripheral(uint8_t _reqn, uint8_t _rdyn, uint16_t _eepromOffset, uint8_t _maxBonds);

  void begin();
  void loop();

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

}

#endif