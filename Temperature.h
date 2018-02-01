#ifndef Temperature_h
#define Temperature_h

#include <BLEServer.h>

class Temperature : public BLECharacteristicCallbacks
{
public:
  Temperature(BLEService *pService, char *serviceUuid, char *characteristicUuid);

  void onWrite(BLECharacteristic *pCharacteristic);
  static uint8_t value;
  static BLECharacteristic *pCharacteristic;

private:
  char *serviceUuid;
  char *characteristicUuid;
  BLEService *pService;
};

#endif