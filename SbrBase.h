#ifndef SbrBase_h
#define SbrBase_h

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
    template <typename T1>
    static T1 constraint(T1 value, T1 minValue, T1 maxValue);

    std::string name;
    TaskHandle_t task;

  protected:
    SbrBase(std::string name);
    virtual void setup() = 0;
    virtual void loop() = 0;

    static T *singleTon;
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
    for (int i = 0; i < aWhile; i++)
    {
        esp_task_wdt_reset();

        delay(1);
    }
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

#endif