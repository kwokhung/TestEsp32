#pragma once

#include <HardwareSerial.h>
#include <BLEServer.h>
#include <BLE2902.h>

class Hid : public BLECharacteristicCallbacks
{
public:
  void onWrite(BLECharacteristic *hidCharacteristic);
  static Hid *getSingleTon(BLEService *keyboardService);
  static void init(BLEService *keyboardService);
  static void setValue(uint8_t *newValue, size_t length);
  static void notify();

private:
  Hid(BLEService *keyboardService);

  BLEService *keyboardService;
  static Hid *singleTon;
  static BLECharacteristic *hidCharacteristic;
  static uint8_t *value;
};
