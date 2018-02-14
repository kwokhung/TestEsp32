#ifndef SbrZZZ_h
#define SbrZZZ_h

#include <PID_v1.h>

#include "Led.h"
#include "SbrBase.h"

class SbrZZZ : public SbrBase<SbrZZZ>
{
public:
  friend class SbrBase;

private:
  SbrZZZ(std::string name)
      : SbrBase(name)
  {
  }
  void setup() override;
  void loop() override;

  double Setpoint, Input, Output;
  double Kp, Ki, Kd;
  Led *myLED;
  PID *myPID;
};

#endif