#ifndef SbrConfig_h
#define SbrConfig_h

#include <string>
#include <string.h>
#include <FreeRTOS.h>
#include <Arduino.h>
#include <WiFi.h>
#include <ESP32WebServer.h>

#include "SbrBase.h"
#include "SbrMotor.h"
#include "SbrPid.h"

class SbrConfig : public SbrBase<SbrConfig>
{
public:
  SbrConfig(std::string name)
      : SbrBase(name)
  {
  }

  void setup() override;
  void loop() override;

  static void displayInfo();

  static ESP32WebServer &server;
  static SbrMotor *sbrMotor;
  static SbrPid *sbrPid;

private:
};

#endif