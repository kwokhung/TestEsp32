#ifndef SbrConfig_h
#define SbrConfig_h

#include <string>
#include <string.h>
#include <FreeRTOS.h>
#include <Arduino.h>
#include <WiFi.h>
#include <ESP32WebServer.h>

#include "SbrMotor.h"

class SbrConfig
{
public:
  SbrConfig(std::string name, SbrMotor *sbrMotor);

  void setup();
  void setup_wifi();

  static void startUp(void *parameter);
  static void displayInfo();
  static void handleNotFound();
  static void updateVars();

  TaskHandle_t task;

  static ESP32WebServer &server;
  static SbrMotor *sbrMotor;

private:
  std::string name;
};

#endif