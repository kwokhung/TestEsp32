#include "Ble.h"

Ble ble("Thermometer", "4fafc201-1fb5-459e-8fcc-c5c9c331914b", "beb5483e-36e1-4688-b7f5-ea07361b26a8");

void setup()
{
    ble.setup();
}

void loop()
{
    ble.notify();
}
