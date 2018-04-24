#include "SbrXXX07.hpp"

void SbrXXX07::setup()
{
    Serial.println("SbrXXX07::setup");

    queueSize = 10;
    queue = xQueueCreate(queueSize, sizeof(int));
}

void SbrXXX07::loop()
{
    Serial.println("SbrXXX07::loop");

    sleepAWhile(1000);
}

HardwareSerial &SbrXXX07::Serial1 = *new HardwareSerial(2);
