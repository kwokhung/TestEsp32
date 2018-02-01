#ifndef Temperature_h
#define Temperature_h

#include <BLEServer.h>

class Temperature : public BLECharacteristicCallbacks
{
public:
  Temperature(BLEService *thermometerService, char *temperatureCharacteristicUuid);

  void onWrite(BLECharacteristic *temperatureCharacteristic);
  static uint8_t getValue();
  static void setValue(uint8_t newValue);
  static void notify();

private:
  char *thermometerServiceUuid;
  char *temperatureCharacteristicUuid;
  BLEService *thermometerService;
  static BLECharacteristic *temperatureCharacteristic;
  static uint8_t value;
};

#endif