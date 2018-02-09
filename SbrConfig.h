#ifndef SbrConfig_h
#define SbrConfig_h

#include <string>
#include <string.h>
#include <FreeRTOS.h>
#include <Arduino.h>
#include <WiFi.h>
#include <ESP32WebServer.h>

#include "SbrMotor.h"
#include "SbrPid.h"

class SbrConfig
{
public:
  void setup();
  void loop();

  static SbrConfig *getSingleTon(std::string name);
  static void startUp(void *parameter);
  static void displayInfo();

  std::string name;
  TaskHandle_t task;

  static ESP32WebServer &server;
  static SbrMotor *sbrMotor;
  static SbrPid *sbrPid;

private:
  SbrConfig(std::string name);

  static SbrConfig *singleTon;
};

#endif