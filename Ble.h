#ifndef Ble_h
#define Ble_h

#include <BLEServer.h>

class Ble
{
public:
  Ble(std::string name, char *thermometerServiceUuid, char *temperatureCharacteristicUuid);

  void setup();
  void notify();

private:
  std::string name;
  char *thermometerServiceUuid;
  char *temperatureCharacteristicUuid;
  BLEServer *bleServer;
};

#endif