#ifndef SbrZZZ_h
#define SbrZZZ_h

#include <PID_v1.h>

#include "SbrXXX.h"
#include "SbrBase.h"

class SbrZZZ : public SbrBase<SbrZZZ>
{
public:
  friend class SbrBase;

  static SbrXXX *sbrXXX;

private:
  SbrZZZ(std::string name)
      : SbrBase(name)
  {
  }
  void setup() override;
  void loop() override;

  double Setpoint, Input, Output;
  double Kp, Ki, Kd;
  PID *myPID;
};

#endif