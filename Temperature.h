#ifndef Temperature_h
#define Temperature_h

#include <BLEServer.h>

class Temperature : public BLECharacteristicCallbacks
{
public:
  Temperature(BLEService *pService, char *temperatureUuid);

  void onWrite(BLECharacteristic *pCharacteristic);
  static uint8_t getValue();
  static void setValue(uint8_t newValue);
  static void notify();
  static uint8_t value;
  static BLECharacteristic *pCharacteristic;

private:
  char *thermometerUuid;
  char *temperatureUuid;
  BLEService *pService;
};

#endif