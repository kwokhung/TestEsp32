#pragma once

#include <HardwareSerial.h>
#include <BLEServer.h>

#include "Hid.hpp"

class Keyboard
{
public:
  static Keyboard *getSingleTon(BLEServer *bleServer, char *keyboardServiceUuid, char *hidCharacteristicUuid);
  static void init(BLEServer *bleServer, char *keyboardServiceUuid, char *hidCharacteristicUuid);

private:
  Keyboard(BLEServer *bleServer, char *keyboardServiceUuid, char *hidCharacteristicUuid);

  char *keyboardServiceUuid;
  char *hidCharacteristicUuid;
  BLEServer *bleServer;
  static Keyboard *singleTon;
  static BLEService *keyboardService;
};
