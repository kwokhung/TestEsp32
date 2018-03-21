#include <Arduino.h>
#include <HardwareSerial.h>
#include <SBUS.h>
//#include <hackflight.hpp>
//#include <boards/sim/sim.hpp>
//#include <receivers/real/serial/arduino_sbus.hpp>

#include "SbrXXX06.h"

void SbrXXX06::setup()
{
    Serial.println("SbrXXX06::setup");

    queueSize = 10;
    queue = xQueueCreate(queueSize, sizeof(int));

    HardwareSerial *Serial1 = new HardwareSerial(2);
    SBUS rx = *new SBUS(*Serial1);

    //HardwareSerial Serial1 = *new HardwareSerial(2);
    //SBUS *a = new SBUS(Serial);
    //SBUS b = *new SBUS(Serial);
    //SBUS c(Serial);
}

void SbrXXX06::loop()
{
    Serial.println("SbrXXX06::loop");

    sleepAWhile(1000);
}
