#include <HardwareSerial.h>
#include <esp_task_wdt.h>

#include "SbrMotor.h"

void SbrMotor::setSpeed(int16_t s, int16_t rotation)
{
    return;

    int16_t sL = s - rotation;
    int16_t sR = s + rotation;
    boolean backwardL = sL < 0;
    boolean backwardR = sR < 0;

    if (backwardL)
    {
        forwardL(false);
        sL = -sL;
    }
    else
    {
        forwardL(true);
    }

    if (backwardR)
    {
        forwardR(false);
        sR = -sR;
    }
    else
    {
        forwardR(true);
    }

    disableL(sL < MAX_SPEED / 100);
    disableR(sR < MAX_SPEED / 100);

    if (sL > MAX_SPEED)
    {
        sL = MAX_SPEED;
    }

    if (sR > MAX_SPEED)
    {
        sR = MAX_SPEED;
    }

    // keep track of the position (in steps forward or backward)
    currentPos += ((micros() - prevSpeedStart) / (float)1000000) * prevSpeed;
    prevSpeed = s;
    prevSpeedStart = micros();

    // set the new speed
    ledcWriteTone(MOT_L_CHANNEL, sL);
    ledcWriteTone(MOT_R_CHANNEL, sR);
}

void SbrMotor::setup()
{
    Serial.println("SbrMotor::setup");

    return;

    ledcSetup(MOT_L_CHANNEL, 0, 10);
    ledcSetup(MOT_R_CHANNEL, 0, 10);

    ledcAttachPin(MOT_L_STP, MOT_L_CHANNEL);
    ledcAttachPin(MOT_R_STP, MOT_R_CHANNEL);

    pinMode(MOT_L_ENB, OUTPUT);
    pinMode(MOT_R_ENB, OUTPUT);

    pinMode(MOT_L_DIR, OUTPUT);
    pinMode(MOT_R_DIR, OUTPUT);

    disableL(true);
    disableR(true);

    currentPos = 0;
}

void SbrMotor::loop()
{
    Serial.println("SbrMotor::loop");

    sleepAWhile(1000);
}

void SbrMotor::disableL(bool orEnable)
{
    digitalWrite(MOT_L_ENB, orEnable);
}

void SbrMotor::disableR(bool orEnable)
{
    digitalWrite(MOT_R_ENB, orEnable);
}

void SbrMotor::forwardL(bool orBack)
{
    digitalWrite(MOT_L_DIR, orBack);
}

void SbrMotor::forwardR(bool orBack)
{
    digitalWrite(MOT_R_DIR, orBack);
}
