#ifndef SbrXXX_h
#define SbrXXX_h

#include "Led.h"
#include "SbrBase.h"

class SbrXXX : public SbrBase<SbrXXX>
{
public:
  friend class SbrBase;

private:
  SbrXXX(std::string name)
      : SbrBase(name)
  {
  }
  void setup() override;
  void loop() override;

  int brightness;
  Led *myLED;
};

#endif