#ifndef Gprs_h
#define Gprs_h

#define TINY_GSM_MODEM_A6

#include "TinyGsmClient.h"

class Gprs
{
public:
  Gprs(char *apn);

  void setup();
  TinyGsmClient *getGsmClient();

private:
    TinyGsm *modem;
    TinyGsmClient *gsmClient;
    char *apn;
  };

#endif