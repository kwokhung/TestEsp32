#ifndef Ble_h
#define Ble_h

#include <BLEServer.h>

class Ble
{
public:
  Ble(std::string name, char *thermometerUuid, char *temperatureUuid);

  void setup();
  void notify();

private:
  std::string name;
  char *thermometerUuid;
  char *temperatureUuid;
  BLEServer *bleServer;
};

#endif