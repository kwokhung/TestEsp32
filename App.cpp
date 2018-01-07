#include <HardwareSerial.h>

#include "Led.h"
#include "OLed.h"
#include "Gprs.h"
#include "App.h"

App::App(HardwareSerial &serialAT)
    : serialAT(&serialAT),
      led(new Led(LED_R_PIN, LED_G_PIN, LED_B_Pin)),
      oLed(new OLed(OLed_ADDRESS, OLed_SDA_PIN, OLed_SCL_PIN)),
      gprs(new Gprs(*this->serialAT, APN))
{
}

void App::setup()
{
    Serial.begin(SERIAL_RATE);

    led->setup();
    oLed->setup();
    oLed->hello();
    gprs->setup();
}
