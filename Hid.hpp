#pragma once

#include <HardwareSerial.h>
#include <BLEServer.h>
#include <BLE2902.h>

class Hid : public BLECharacteristicCallbacks
{
public:
  void onWrite(BLECharacteristic *hidCharacteristic);
  static Hid *getSingleTon(BLEService *keyboardService, char *hidCharacteristicUuid);
  static void init(BLEService *keyboardService, char *hidCharacteristicUuid);
  static uint8_t getValue();
  static void setValue(uint8_t newValue);
  static void notify();

private:
  Hid(BLEService *keyboardService, char *hidCharacteristicUuid);

  char *keyboardServiceUuid;
  char *hidCharacteristicUuid;
  BLEService *keyboardService;
  static Hid *singleTon;
  static BLECharacteristic *hidCharacteristic;
  static uint8_t value;
};
