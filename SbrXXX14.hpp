#pragma once

#include <Arduino.h>
#include <HardwareSerial.h>
#include <WiFi.h>
#include <WifiEspNowBroadcast.h>

#include "SbrBase.hpp"

class SbrXXX14 : public SbrBase<SbrXXX14>
{
public:
  friend class SbrBase;

private:
  SbrXXX14(std::string name)
      : SbrBase(name)
  {
  }
  void setup() override;
  void loop() override;
};
