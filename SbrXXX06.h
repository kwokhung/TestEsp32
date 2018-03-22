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

  static hf::Hackflight &h;
  static hf::SBUS_Receiver &rc;
  static hf::Stabilizer &stabilizer;

private:
  SbrXXX06(std::string name)
      : SbrBase(name)
  {
  }
  void setup() override;
  void loop() override;
};

#endif