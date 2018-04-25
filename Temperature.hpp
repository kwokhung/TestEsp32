#pragma once

#include <HardwareSerial.h>
#include <BLEServer.h>
#include <BLE2902.h>

class Temperature : public BLECharacteristicCallbacks
{
public:
  void onWrite(BLECharacteristic *temperatureCharacteristic);
  static void init(BLEService *thermometerService, char *temperatureCharacteristicUuid);
  static uint8_t getValue();
  static void setValue(uint8_t newValue);
  static void notify();

private:
  Temperature(BLEService *thermometerService, char *temperatureCharacteristicUuid);

  char *thermometerServiceUuid;
  char *temperatureCharacteristicUuid;
  BLEService *thermometerService;
  static bool initted;
  static BLECharacteristic *temperatureCharacteristic;
  static uint8_t value;
};
