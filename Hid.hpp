#pragma once

#include <HardwareSerial.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <BLE2904.h>

class Hid : public BLECharacteristicCallbacks
{
public:
  static Hid *getSingleTon(BLEService *deviceInformationService, BLEService *humanInterfaceDeviceService, BLEService *batteryService);
  static void init(BLEService *deviceInformationService, BLEService *humanInterfaceDeviceService, BLEService *batteryService);
  static void setValue(BLECharacteristic *mouseInputReport, uint8_t *newValue, size_t length);
  static void notify(BLECharacteristic *mouseInputReport);
  static void sendKey(uint8_t modifier, uint8_t key);
  static void sendKey(uint8_t key);
  static void sendMouse(uint8_t buttons, int8_t x, int8_t y, int8_t wheel);

private:
  Hid(BLEService *deviceInformationService, BLEService *humanInterfaceDeviceService, BLEService *batteryService);

  BLEService *deviceInformationService;
  BLEService *humanInterfaceDeviceService;
  BLEService *batteryService;
  static Hid *singleTon;
  static BLECharacteristic *pnpId;
  static BLECharacteristic *manufacturerNameString;
  static BLECharacteristic *hidInformation;
  static BLECharacteristic *reportMap;
  static BLECharacteristic *hidControlPoint;
  static BLECharacteristic *keyboardInputReport;
  static BLECharacteristic *mouseInputReport;
  static BLECharacteristic *keyboardOutputReport;
  static BLECharacteristic *mouseOutputReport;
  static BLECharacteristic *keyboardFeatureReport;
  static BLECharacteristic *mouseFeatureReport;
  static BLECharacteristic *protocolMode;
  static BLECharacteristic *bootKeyboardInputReport;
  static BLECharacteristic *bootKeyboardOutputReport;
  static BLECharacteristic *bootMouseInputReport;
  static BLECharacteristic *batteryLevel;
};
