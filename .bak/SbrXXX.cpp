#include <HardwareSerial.h>

#include "SbrXXX.h"

void SbrXXX::setup()
{
    Serial.println("SbrXXX::setup");

    queueSize = 10;
    queue = xQueueCreate(queueSize, sizeof(int));

    myLED = new Led(27, 26, 25);
    myLED->setup();
}

void SbrXXX::loop()
{
    Serial.println("SbrXXX::loop");

    xQueueReceive(queue, &brightness, portMAX_DELAY);
    myLED->lightG(brightness);

    //sleepAWhile(1000);
}
