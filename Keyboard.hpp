#pragma once

#include <HardwareSerial.h>
#include <BLEServer.h>

#include "Hid.hpp"

class Keyboard
{
public:
  static Keyboard *getSingleTon(BLEServer *bleServer);
  static void init(BLEServer *bleServer);

private:
  Keyboard(BLEServer *bleServer);

  BLEServer *bleServer;
  static Keyboard *singleTon;
  static BLEService *deviceInformationService;
  static BLEService *humanInterfaceDeviceService;
};
