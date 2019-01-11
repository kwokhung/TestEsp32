#ifndef Mqtt_h
#define Mqtt_h

#include "Gprs.h"
#include <PubSubClient.h>

#include "Led.h"
#include "OLed.h"

class Mqtt
{
public:
  Mqtt(Gprs &gprs, char *mqttUrl, Led &led, OLed &oLed);

  void setup();
  void loop();
  void reconnect();

private:
  Gprs *gprs;
  PubSubClient *client;
  char *mqttUrl;
  Led *led;
  OLed *oLed;
};

#endif