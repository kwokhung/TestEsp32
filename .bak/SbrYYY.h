#ifndef SbrYYY_h
#define SbrYYY_h

#include "SbrBase.h"

class SbrYYY : public SbrBase<SbrYYY>
{
public:
  friend class SbrBase;

private:
  SbrYYY(std::string name)
      : SbrBase(name)
  {
  }
  void setup() override;
  void loop() override;
};

#endif