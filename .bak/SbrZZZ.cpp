#include <HardwareSerial.h>

#include "SbrZZZ.h"

void SbrZZZ::setup()
{
    Serial.println("SbrZZZ::setup");

    Setpoint = 50;
    Kp = 2;
    Ki = 5;
    Kd = 1;

    myPID = new PID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

    myPID->SetSampleTime(1000);
    myPID->SetOutputLimits(0, 1023);
    myPID->SetMode(AUTOMATIC);
    myPID->SetTunings(10, 5, 1);
}

void SbrZZZ::loop()
{
    Serial.println("SbrZZZ::loop");

    Input = touchRead(15); // Just test touch pin - Touch3 is T3 which is on GPIO 15.
    Serial.printf("Input: %f\n", Input);

    myPID->Compute();

    int brightness = (int)Output;
    Serial.printf("Output: %d\n", brightness);

    xQueueSend(sbrXXX->queue, &brightness, portMAX_DELAY);

    //sleepAWhile(1000);
}

SbrXXX *SbrZZZ::sbrXXX;
