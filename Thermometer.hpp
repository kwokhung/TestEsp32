#pragma once

#include <HardwareSerial.h>
#include <BLEServer.h>

#include "Temperature.hpp"

class Thermometer
{
public:
  static Thermometer *getSingleTon(BLEServer *bleServer, char *thermometerServiceUuid, char *temperatureCharacteristicUuid);
  static void init(BLEServer *bleServer, char *thermometerServiceUuid, char *temperatureCharacteristicUuid);

private:
  Thermometer(BLEServer *bleServer, char *thermometerServiceUuid, char *temperatureCharacteristicUuid);

  char *thermometerServiceUuid;
  char *temperatureCharacteristicUuid;
  BLEServer *bleServer;
  static Thermometer *singleTon;
  static BLEService *thermometerService;
};
