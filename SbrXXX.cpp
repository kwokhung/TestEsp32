#include <HardwareSerial.h>

#include "SbrXXX.h"

void SbrXXX::setup()
{
    Serial.println("SbrXXX::setup");
}

void SbrXXX::loop()
{
    Serial.println("SbrXXX::loop");

    sleepAWhile(1000);
}
