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

  //static SbrConfig *getSingleTon(std::string name);
  //static void startUp(void *parameter);
  static void displayInfo();

  //std::string name;
  //TaskHandle_t task;

  static ESP32WebServer &server;
  static SbrMotor *sbrMotor;
  static SbrPid *sbrPid;

private:
  //SbrConfig(std::string name);

  //static SbrConfig *singleTon;
};

#endif