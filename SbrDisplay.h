#ifndef SbrDisplay_h
#define SbrDisplay_h

#include <string>
#include <string.h>
#include <FreeRTOS.h>
#include <Arduino.h>
#include <WiFi.h>
#include <ESP32WebServer.h>

#define MAX_SPEED 20000

class SbrDisplay
{
public:
  SbrDisplay(std::string name);

  void setup();
  void setup_wifi();

  static void startUp(void *parameter);
  static void displayInfo();
  static void handleNotFound();
  static void updateVars();

  TaskHandle_t task;

  static HardwareSerial &SerialControl;
  static ESP32WebServer &server;

private:
  std::string name;
};

#endif