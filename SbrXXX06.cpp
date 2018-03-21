#include <Arduino.h>
#include <HardwareSerial.h>
#include "Sbus.h"
//#include <hackflight.hpp>
//#include <boards/sim/sim.hpp>
//#include <receivers/real/serial/arduino_sbus.hpp>

#include "SbrXXX06.h"

void SbrXXX06::setup()
{
    Serial.println("SbrXXX06::setup");

    queueSize = 10;
    queue = xQueueCreate(queueSize, sizeof(int));

    Sbus &rx = *new Sbus(*new HardwareSerial(2));
}

void SbrXXX06::loop()
{
    Serial.println("SbrXXX06::loop");

    sleepAWhile(1000);
}
