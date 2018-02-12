#ifndef SbrXXX_h
#define SbrXXX_h

#include "SbrBase.h"

class SbrXXX : public SbrBase<SbrXXX>
{
public:
  SbrXXX(std::string name)
      : SbrBase(name)
  {
  }

  void setup() override;
  void loop() override;

private:
};

#endif