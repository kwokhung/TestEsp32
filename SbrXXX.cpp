#include <HardwareSerial.h>

#include "SbrXXX.h"

void SbrXXX::light(int brightness)
{
    myLED->lightG(brightness);
}

void SbrXXX::setup()
{
    Serial.println("SbrXXX::setup");

    myLED = new Led(27, 26, 25);
    myLED->setup();
}

void SbrXXX::loop()
{
    Serial.println("SbrXXX::loop");

    //myLED->lightG(20);

    sleepAWhile(1000);
}
