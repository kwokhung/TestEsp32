#pragma once

#include <Arduino.h>
#include <HardwareSerial.h>
#include <BLEDevice.h>
#include <BLEHidDevice.h>
#include <HIDKeyboardTypes.h>
#include <BluetoothSerial.h>

#include "SbrBase.hpp"
#include "Keyboard.hpp"
#include "Hid.hpp"

class SbrXXX13 : public SbrBase<SbrXXX13>, public BLEServerCallbacks
{
public:
  friend class SbrBase;

  void onConnect(BLEServer *bleServer);
  void onDisconnect(BLEServer *bleServer);
  static bool isConnected();

private:
  SbrXXX13(std::string name)
      : SbrBase(name)
  {
  }
  void setup() override;
  void loop() override;

  double Input;
  BLEServer *bleServer;
  BluetoothSerial *serialBT;
  static uint32_t sleepAWhileCount;
  static bool connected;
  static int8_t x;
  static int8_t y;
};
