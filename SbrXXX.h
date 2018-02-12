#ifndef SbrXXX_h
#define SbrXXX_h

#include "SbrBase.h"

class SbrXXX : public SbrBase<SbrXXX>
{
public:
  friend class SbrBase;
  void setup() override;
  void loop() override;

private:
  SbrXXX(std::string name)
      : SbrBase(name)
  {
  }
};

#endif