#ifndef SbrYYY_h
#define SbrYYY_h

#include "SbrBase.h"

class SbrYYY : public SbrBase<SbrYYY>
{
public:
  friend class SbrBase;
  void setup() override;
  void loop() override;

private:
  SbrYYY(std::string name)
      : SbrBase(name)
  {
  }
};

#endif