#ifndef Ble_h
#define Ble_h

#include <BLEServer.h>

class Ble
{
public:
  Ble(std::string name, char *serviceUuid, char *characteristicUuid);

  void setup();
  void notify();

private:
  std::string name;
  char *serviceUuid;
  char *characteristicUuid;
  BLEServer *pServer;
};

#endif