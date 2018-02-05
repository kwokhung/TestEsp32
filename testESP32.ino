#include <esp_freertos_hooks.h>

#include "Ble.h"

Ble *ble = new Ble("Thermometer", "4fafc201-1fb5-459e-8fcc-c5c9c331914b", "beb5483e-36e1-4688-b7f5-ea07361b26a8");
int counter = 0;

void setup()
{
    Serial.begin(115200);

    esp_register_freertos_idle_hook(vApplicationIdleHook);

    uint32_t delayTime = 1000;

    xTaskCreate([&](void *parameter) {
        uint32_t delayTime = *(uint32_t *)parameter;

        Serial.printf("Parameter: %d\n", delayTime);

        for (;;)
        {
            Serial.println("I am here.");

            vTaskDelay(pdMS_TO_TICKS(delayTime));
        }

        vTaskDelete(NULL);
    },
                "I Am Here", 10000, &delayTime, 1, NULL);

    char task1Param[12] = "task1Param";

    xTaskCreate(
        task1,              /* Task function. */
        "task1",            /* name of task. */
        10000,              /* Stack size of task */
        (void *)task1Param, /* parameter of the task */
        1,                  /* priority of the task */
        NULL);

    xTaskCreate(Ble::startUp, "BLE", 10000, ble, 1, &ble->task);

    //vTaskStartScheduler();
}

void loop()
{
}

void task1(void *parameter)
{
    Serial.println((char *)parameter);
    /* loop forever */
    for (;;)
    {
        Serial.print("task1 is running and counter : ");
        Serial.println(counter);
        if (counter > 30000)
        {
            counter = 0;
        }
        /* block this task so that Idle task has chance to run */
        delay(10);
    }
    /* delete a task when finish, 
  this will never happen because this is infinity loop */
    vTaskDelete(NULL);
}

bool vApplicationIdleHook(void)
{
    counter++;
}
