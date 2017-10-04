#include <Arduino.h>

#include "Led.h"

Led::Led(int rPin, int gPin, int bPin)
    : rPin(rPin),
      gPin(gPin),
      bPin(bPin)
{
}

void Led::setup()
{
    pinMode(rPin, OUTPUT);
    pinMode(gPin, OUTPUT);
    pinMode(bPin, OUTPUT);
    digitalWrite(rPin, HIGH);
    digitalWrite(gPin, HIGH);
    digitalWrite(bPin, HIGH);
}

void Led::light(int pin, int brightness)
{
    if (brightness > 0)
    {
        analogWrite(pin, 1023 - brightness);
    }
    else if (brightness == 0)
    {
        analogWrite(pin, 1023);
        digitalWrite(pin, HIGH);
    }
}

void Led::lightR(int brightness)
{
    light(rPin, brightness);
}

void Led::lightG(int brightness)
{
    light(gPin, brightness);
}

void Led::lightB(int brightness)
{
    light(bPin, brightness);
}
