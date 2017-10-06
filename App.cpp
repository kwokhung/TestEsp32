#include <Preferences.h>
#include <HardwareSerial.h>

#include "Led.h"
#include "OLed.h"
#include "Gprs.h"
#include "Mqtt.h"
#include "App.h"

App::App()
    : preferences(new Preferences()),
      led(new Led(LED_R_PIN, LED_G_PIN, LED_B_Pin)),
      oLed(new OLed(OLed_ADDRESS, OLed_SDA_PIN, OLed_SCL_PIN)),
      //wifi(new Wifi(*oLed)),
      gprs(new Gprs(APN)),
      mqtt(new Mqtt(*gprs, MQTT_URL, *led, *oLed))
{
}

void App::setup()
{
    preferences->begin("App", false);
    preferences->putString("Name", "testESP32");
    preferences->end();

    Serial.begin(SERIAL_RATE);

    led->setup();
    oLed->setup();
    oLed->hello();
    //wifi->setup();
    gprs->setup();
    //mqtt->setup();
}

Mqtt *App::getMqtt()
{
    return mqtt;
}
