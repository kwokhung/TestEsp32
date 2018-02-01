#ifndef Temperature_h
#define Temperature_h

#include <BLEServer.h>

class Temperature : public BLECharacteristicCallbacks
{
  public:
    void onWrite(BLECharacteristic *pCharacteristic);
    static uint8_t value;
};

#endif