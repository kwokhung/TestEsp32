#ifndef SbrXXX06_h
#define SbrXXX06_h

#include <Arduino.h>
#include <HardwareSerial.h>
#include <hackflight.hpp>
#include <stabilizer.hpp>
#include <boards/sim/sim.hpp>
#include <receivers/real/serial/arduino_sbus.hpp>

#include "SbrBase.h"

class SbrXXX06 : public SbrBase<SbrXXX06>
{
public:
  friend class SbrBase;

private:
  SbrXXX06(std::string name)
      : SbrBase(name)
  {
  }
  void setup() override;
  void loop() override;

  hf::Hackflight h;
  hf::SBUS_Receiver rc;
  hf::Stabilizer stabilizer;
};

#endif