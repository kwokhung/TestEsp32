#ifndef Ble_h
#define Ble_h

#include <BLEServer.h>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

class Ble
{
public:
  Ble(std::string name);

  void setup();
  void notify();

private:
  std::string name;
  BLEServer *pServer;
  BLEService *pService;
  BLECharacteristic *pCharacteristic;
};

#endif