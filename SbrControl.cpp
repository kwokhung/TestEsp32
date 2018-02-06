#include <HardwareSerial.h>
#include <Wire.h>

#include "SbrControl.h"

SbrControl::SbrControl(std::string name)
    : name(name)
{
    task = NULL;
}

void SbrControl::setup()
{
    Serial.println("SbrControl::setup");

    setup_motor();
}

void SbrControl::setup_motor()
{
    ledcAttachPin(MOT_L_STP, MOT_L_CHANNEL);
    ledcSetup(MOT_L_CHANNEL, 0, 10); // these will be updated later by the ledcWriteNote()
    ledcAttachPin(MOT_R_STP, MOT_R_CHANNEL);
    ledcSetup(MOT_R_CHANNEL, 0, 10); // these will be updated later by the ledcWriteNote()

    pinMode(MOT_L_ENB, OUTPUT);
    pinMode(MOT_L_DIR, OUTPUT);
    disableL(true);

    pinMode(MOT_R_ENB, OUTPUT);
    pinMode(MOT_R_DIR, OUTPUT);
    disableR(true);

    currentPos = 0;
}

void SbrControl::disableL(bool orEnable)
{
    digitalWrite(MOT_L_ENB, orEnable);
}

void SbrControl::disableR(bool orEnable)
{
    digitalWrite(MOT_R_ENB, orEnable);
}

void SbrControl::forwardL(bool orBack)
{
    digitalWrite(MOT_L_DIR, orBack);
}

void SbrControl::forwardR(bool orBack)
{
    digitalWrite(MOT_R_DIR, orBack);
}

void SbrControl::setSpeed(int16_t s, int16_t rotation)
{
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
        sL = MAX_SPEED;
    if (sR > MAX_SPEED)
        sR = MAX_SPEED;

    // keep track of the position (in steps forward or backward)
    currentPos += ((micros() - prevSpeedStart) / (float)1000000) * prevSpeed;
    prevSpeed = s;
    prevSpeedStart = micros();

    // set the new speed
    ledcWriteTone(MOT_L_CHANNEL, sL);
    ledcWriteTone(MOT_R_CHANNEL, sR);
}

void SbrControl::startUp(void *parameter)
{
    SbrControl *sbrControl = (SbrControl *)parameter;

    sbrControl->setup();

    while (true)
    {
    }

    vTaskDelete(NULL);
}
