#ifndef SbrXXX03_h
#define SbrXXX03_h

#include <MPU6050.h>

#include "SbrBase.h"

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
  float axInG, ayInG, azInG;
  int16_t gx, gy, gz;
  float gxInD, gyInD, gzInD;
};

#endif