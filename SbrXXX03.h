#ifndef SbrXXX03_h
#define SbrXXX03_h

#include <MPU6050.h>

#include "SbrBase.h"

#define OUTPUT_READABLE_ACCELGYRO
#define OUTPUT_BINARY_ACCELGYRO

class SbrXXX03 : public SbrBase<SbrXXX03>
{
public:
  friend class SbrBase;

private:
  SbrXXX03(std::string name)
      : SbrBase(name)
  {
  }
  void setup() override;
  void loop() override;

  MPU6050 *accelgyro;
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
};

#endif