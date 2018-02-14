#ifndef SbrXXX_h
#define SbrXXX_h

#include "Led.h"
#include "SbrBase.h"

class SbrXXX : public SbrBase<SbrXXX>
{
public:
  friend class SbrBase;
  void light(int brightness);

private:
  SbrXXX(std::string name)
      : SbrBase(name)
  {
  }
  void setup() override;
  void loop() override;

  Led *myLED;
};

#endif