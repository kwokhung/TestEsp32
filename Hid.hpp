#pragma once

#include <HardwareSerial.h>
#include <BLEServer.h>
#include <BLE2902.h>

class Hid : public BLECharacteristicCallbacks
{
public:
  static Hid *getSingleTon(BLEService *deviceInformationService, BLEService *humanInterfaceDeviceService);
  static void init(BLEService *deviceInformationService, BLEService *humanInterfaceDeviceService);
  static void setValue(uint8_t *newValue, size_t length);
  static void notify();

private:
  Hid(BLEService *deviceInformationService, BLEService *humanInterfaceDeviceService);

  BLEService *deviceInformationService;
  BLEService *humanInterfaceDeviceService;
  static Hid *singleTon;
  static BLECharacteristic *pnpId;
  static BLECharacteristic *manufacturerNameString;
  static BLECharacteristic *hidInformation;
  static BLECharacteristic *reportMap;
  static BLECharacteristic *hidControlPoint;
  static BLECharacteristic *report1;
  static BLECharacteristic *report2;
  static BLECharacteristic *report3;
  static BLECharacteristic *protocolMode;
  static BLECharacteristic *bootKeyboardInputReport;
  static BLECharacteristic *bootKeyboardOutputReport;
  static uint8_t *value;
};
