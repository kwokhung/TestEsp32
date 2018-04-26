#pragma once

#include <HardwareSerial.h>
#include <BLEServer.h>
#include <BLE2902.h>

class Hid : public BLECharacteristicCallbacks
{
public:
  static Hid *getSingleTon(BLEService *keyboardService, BLEService *keyboardService1);
  static void init(BLEService *keyboardService, BLEService *keyboardService1);
  static void setValue(uint8_t *newValue, size_t length);
  static void notify();

private:
  Hid(BLEService *keyboardService, BLEService *keyboardService1);

  BLEService *keyboardService;
  BLEService *keyboardService1;
  static Hid *singleTon;
  static BLECharacteristic *manufacturer;
  static BLECharacteristic *pnpIDChar;
  static BLECharacteristic *hidInfoChar;
  static BLECharacteristic *reportMapChar;
  static BLECharacteristic *hidControlChar;
  static BLECharacteristic *reportChar1;
  static BLECharacteristic *reportChar2;
  static BLECharacteristic *reportChar3;
  static BLECharacteristic *protocolModeChar;
  static BLECharacteristic *bootInputChar;
  static BLECharacteristic *bootOutputChar;
  static uint8_t *value;
};
