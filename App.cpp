#include <Preferences.h>
#include <HardwareSerial.h>

#include "App.h"

App::App()
{
}

void App::setup()
{
    Serial.begin(SERIAL_RATE);

}
