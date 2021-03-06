#pragma once

#include <Arduino.h>
#include <HardwareSerial.h>
#include <BLEDevice.h>

#include "SbrBase.hpp"
#include "Thermometer.hpp"
#include "Temperature.hpp"

class SbrXXX07 : public SbrBase<SbrXXX07>, public BLEServerCallbacks
{
public:
  friend class SbrBase;

  void onConnect(BLEServer *bleServer);
  void onDisconnect(BLEServer *bleServer);
  static bool isConnected();

private:
  SbrXXX07(std::string name)
      : SbrBase(name)
  {
  }
  void setup() override;
  void loop() override;

  BLEServer *bleServer;
  static bool connected;
};
