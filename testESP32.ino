#include "Ble.h"

#include "SbrBase.h"

class SbrXXX : public SbrBase<SbrXXX>
{
public:
  SbrXXX(std::string name)
      : SbrBase(name)
  {
  }

  void setup() override;
  void loop() override;

private:
};

void SbrXXX::setup()
{
}

void SbrXXX::loop()
{
}

SbrXXX *sbrXXX = SbrXXX::getSingleTon("SBR - XXX");

Ble *ble = new Ble("Thermometer", "4fafc201-1fb5-459e-8fcc-c5c9c331914b", "beb5483e-36e1-4688-b7f5-ea07361b26a8");

void setup()
{
  Serial.begin(115200);

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

  xTaskCreate(Ble::startUp, "BLE", 10000, ble, 1, &ble->task);

  //vTaskStartScheduler();
}

void loop()
{
}
