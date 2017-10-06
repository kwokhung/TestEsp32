#ifndef App_h
#define App_h

#define EEPROM_SIZE 512

#define SERIAL_RATE 115200
#define SERIAL_RX0_PIN 3 // GPIO3 / RX0 / D0
#define SERIAL_TX0_PIN 1 // GPIO1 / TX0 / D1

#define LED_ONBOARD_PIN 2 // GPIO2 / E4 / D9

#define LED_R_PIN 27
#define LED_G_PIN 26
#define LED_B_Pin 25

#define OLed_ADDRESS 0x3c
#define OLed_SDA_PIN 21
#define OLed_SCL_PIN 22

#define APN "mobile.lte.three.com.hk"

#define MQTT_URL "mbltest01.mqtt.iot.gz.baidubce.com"

#include "Led.h"
#include "OLed.h"
#include "Gprs.h"

class App
{
public:
  App();

  void setup();

private:
  Led *led;
  OLed *oLed;
  Gprs *gprs;
};

#endif