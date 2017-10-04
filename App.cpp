#include <Preferences.h>
#include <HardwareSerial.h>

#include "Led.h"
#include "App.h"

App::App()
    : led(new Led(LED_R_PIN, LED_G_PIN, LED_B_Pin))
{
}

void App::setup()
{
    Serial.begin(SERIAL_RATE);

    led->setup();
}
