#ifndef Gprs_h
#define Gprs_h

#include <HardwareSerial.h>
#define TINY_GSM_MODEM_A6
#include "TinyGsmClient.h"

class Gprs
{
public:
  Gprs(HardwareSerial &serialAT, char *apn);

  void setup();
  TinyGsmClient *getGsmClient();

private:
    HardwareSerial *serialAT;
    TinyGsm *modem;
    TinyGsmClient *gsmClient;
    char *apn;
  };

#endif