#ifndef SbrXXX07_h
#define SbrXXX07_h

#include <Arduino.h>
#include <HardwareSerial.h>

#include "SbrBase.hpp"

class SbrXXX07 : public SbrBase<SbrXXX07>
{
public:
  friend class SbrBase;

  static HardwareSerial &Serial1;

private:
  SbrXXX07(std::string name)
      : SbrBase(name)
  {
  }
  void setup() override;
  void loop() override;
};

#endif