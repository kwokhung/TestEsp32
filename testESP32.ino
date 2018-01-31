#include "Ble.h"

Ble ble("Thermometer");

void setup()
{
    ble.setup();
}

void loop()
{
    ble.notify();
}
