#include <HardwareSerial.h>

#include "Led.h"
#include "SbrZZZ.h"

void SbrZZZ::setup()
{
    Serial.println("SbrZZZ::setup");

    myLED = new Led(27, 26, 25);

    Setpoint = 50;
    Kp = 2;
    Ki = 5;
    Kd = 1;

    myPID = new PID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

    myPID->SetSampleTime(1000);
    myPID->SetOutputLimits(0, 1023);
    myPID->SetMode(AUTOMATIC);
}

void SbrZZZ::loop()
{
    Serial.println("SbrZZZ::loop");

    Input = touchRead(15); // Just test touch pin - Touch3 is T3 which is on GPIO 15.
    Serial.printf("Input: %f\n", Input);

    myPID->Compute();
    Serial.printf("Output: %f\n", Output);
    int brightness = (int)Output;
    myLED->lightG(brightness);

    sleepAWhile(1000);
}
