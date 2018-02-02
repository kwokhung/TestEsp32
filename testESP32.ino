#include "Ble.h"

Ble ble("Thermometer", "4fafc201-1fb5-459e-8fcc-c5c9c331914b", "beb5483e-36e1-4688-b7f5-ea07361b26a8");

void setup()
{
    Serial.begin(115200);

    uint32_t delayTime = 1000;

    xTaskCreate(iAmHereTask, "I Am Here", 10000, &delayTime, 1, NULL);
    xTaskCreate(Ble::startUp, "BLE", 10000, &ble, 1, NULL);
}

void loop()
{
}

void iAmHereTask(void *parameter)
{
    uint32_t delayTime = *(uint32_t *)parameter;

    Serial.printf("Parameter: %d\n", delayTime);

    for (;;)
    {
        Serial.println("I am here.");

        delay(delayTime);
    }

    vTaskDelete(NULL);
}