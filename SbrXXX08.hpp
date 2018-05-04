#pragma once

#include <Arduino.h>
#include <HardwareSerial.h>
#include <BLEDevice.h>

#include "SbrBase.hpp"
#include "Keyboard.hpp"
#include "Hid.hpp"

class SbrXXX08 : public SbrBase<SbrXXX08>, public BLEServerCallbacks
{
public:
  friend class SbrBase;

  void onConnect(BLEServer *bleServer);
  void onDisconnect(BLEServer *bleServer);
  static bool isConnected();

private:
  SbrXXX08(std::string name)
      : SbrBase(name)
  {
  }
  void setup() override;
  void loop() override;

  double Input;
  BLEServer *bleServer;
  static bool connected;
  static int8_t x;
  static int8_t y;
};
