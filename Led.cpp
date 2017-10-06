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
    ledcSetup(0, 5000, 13);
    ledcSetup(1, 5000, 13);
    ledcSetup(2, 5000, 13);

    lightR(0);
    lightG(0);
    lightB(0);
}

void Led::analogWrite(uint8_t pin, int val)
{
    // calculate duty, 8191 from 2 ^ 13 - 1
    uint32_t duty = (8191 / 1023) * min(val, 1023);

    if (pin == rPin)
    {
        if (val == 1023)
        {
            ledcDetachPin(rPin);
            pinMode(rPin, OUTPUT);
            digitalWrite(rPin, HIGH);
        }
        else
        {
            ledcAttachPin(rPin, 0);
            ledcWrite(0, duty);
        }
    }
    else if (pin == gPin)
    {
        if (val == 1023)
        {
            ledcDetachPin(gPin);
            pinMode(gPin, OUTPUT);
            digitalWrite(gPin, HIGH);
        }
        else
        {
            ledcAttachPin(gPin, 1);
            ledcWrite(1, duty);
        }
    }
    else if (pin == bPin)
    {
        if (val == 1023)
        {
            ledcDetachPin(bPin);
            pinMode(bPin, OUTPUT);
            digitalWrite(bPin, HIGH);
        }
        else
        {
            ledcAttachPin(bPin, 2);
            ledcWrite(2, duty);
        }
    }
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
