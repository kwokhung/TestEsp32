#pragma once

#include <string>
#include <FreeRTOS.h>
#include <esp_task_wdt.h>

template <typename T>
class SbrBase
{
  public:
    static T *getSingleTon(std::string name);

    static void startUp(void *parameter);
    static void sleepAWhile(uint32_t aWhile);
    static void sleepAShortWhile(uint32_t aWhile);
    static void displayBuffer(uint8_t *buffer, int length);
    template <typename T1>
    static T1 constraint(T1 value, T1 minValue, T1 maxValue);

    std::string name;
    TaskHandle_t task;
    QueueHandle_t queue;

  protected:
    SbrBase(std::string name);
    virtual void setup() = 0;
    virtual void loop() = 0;

    static T *singleTon;

    int queueSize;
};

template <typename T>
T *SbrBase<T>::getSingleTon(std::string name)
{
    if (singleTon == NULL)
    {
        singleTon = new T(name);
    }

    return (singleTon);
}

template <typename T>
void SbrBase<T>::startUp(void *parameter)
{
    T *sbrBase = (T *)parameter;

    sbrBase->setup();

    esp_task_wdt_add(sbrBase->task);

    while (true)
    {
        sbrBase->loop();

        esp_task_wdt_reset();
    }

    vTaskDelete(NULL);
}

template <typename T>
void SbrBase<T>::sleepAWhile(uint32_t aWhile)
{
    if (aWhile == 0)
    {
        esp_task_wdt_reset();
    }
    else
    {
        for (int i = 0; i < aWhile; i++)
        {
            esp_task_wdt_reset();

            delay(1);
        }
    }
}

template <typename T>
void SbrBase<T>::sleepAShortWhile(uint32_t aShortWhile)
{
    if (aShortWhile == 0)
    {
        esp_task_wdt_reset();
    }
    else
    {
        for (int i = 0; i < aShortWhile; i++)
        {
            esp_task_wdt_reset();

            delayMicroseconds(1);
        }
    }
}

template <typename T>
void SbrBase<T>::displayBuffer(uint8_t *buffer, int length)
{
    int i;

    for (i = 0; i < length; i++)
    {
        Serial.printf("%02x ", buffer[i]);

        if ((i + 1) % 16 == 0)
        {
            Serial.printf("\n");
        }
    }

    Serial.printf("\n");
}

template <typename T>
template <typename T1>
T1 SbrBase<T>::constraint(T1 value, T1 minValue, T1 maxValue)
{
    return (value < minValue ? minValue : (value > maxValue ? maxValue : value));
}

template <typename T>
SbrBase<T>::SbrBase(std::string name)
    : name(name)
{
    task = NULL;
}

template <typename T>
T *SbrBase<T>::singleTon = NULL;
