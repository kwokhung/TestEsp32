#ifndef Wifi_h
#define Wifi_h

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "OLed.h"

class Wifi
{
public:
  Wifi(OLed &oLed);

  void setup();

private:
  ESP8266WebServer *server;
  OLed *oLed;
  String connectInfo;
};

#endif