#pragma once

#include <Arduino.h>
#include <HardwareSerial.h>
#include <BLEDevice.h>
#include <BLEHidDevice.h>

#include "SbrBase.hpp"
#include "Keyboard.hpp"
#include "Hid.hpp"

class SbrXXX09 : public SbrBase<SbrXXX09>, public BLEServerCallbacks
{
public:
  friend class SbrBase;

  void onConnect(BLEServer *bleServer);
  void onDisconnect(BLEServer *bleServer);
  static bool isConnected();

private:
  SbrXXX09(std::string name)
      : SbrBase(name)
  {
  }
  void setup() override;
  void loop() override;

  BLEServer *bleServer;
  BLEHIDDevice *hid;
  BLECharacteristic *input;
  static uint32_t sleepAWhileCount;
  static bool connected;
};
