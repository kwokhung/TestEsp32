#ifndef SbrXXX01_h
#define SbrXXX01_h

#include <Wire.h>
#include <esp_log.h>
#include <driver/gpio.h>
#include "SbrBase.h"

#define SDAPIN (GPIO_NUM_21)
#define SCLPIN (GPIO_NUM_22)

class SbrXXX01 : public SbrBase<SbrXXX01>
{
public:
  friend class SbrBase;

private:
  SbrXXX01(std::string name)
      : SbrBase(name)
  {
  }
  void setup() override;
  void loop() override;

  const int MPU_addr = 0x68; // I2C address of the MPU-6050
  int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
};

#endif