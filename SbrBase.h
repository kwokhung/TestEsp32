#ifndef SbrBase_h
#define SbrBase_h

#include <string>
#include <esp_task_wdt.h>

template <typename T>
class SbrBase
{
  public:
    virtual void setup();
    virtual void loop();

    static T *getSingleTon(std::string name);
    static void startUp(void *parameter);
    static void sleepAWhile(uint32_t aWhile);

    std::string name;
    TaskHandle_t task;

  protected:
    SbrBase(std::string name);

    static T *singleTon;
};

template <typename T>
SbrBase<T>::SbrBase(std::string name)
    : name(name)
{
    task = NULL;
}

template <typename T>
void SbrBase<T>::setup()
{
}

template <typename T>
void SbrBase<T>::loop()
{
}

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
T *SbrBase<T>::singleTon = NULL;

#endif