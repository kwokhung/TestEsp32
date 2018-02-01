#ifndef Thermometer_h
#define Thermometer_h

#include <BLEServer.h>

class Thermometer : public BLEServerCallbacks
{
public:
  Thermometer(BLEServer *bleServer, char *thermometerServiceUuid, char *temperatureCharacteristicUuid);

  void onConnect(BLEServer *bleServer);
  void onDisconnect(BLEServer *bleServer);
  static bool isConnected();

private:
  char *thermometerServiceUuid;
  char *temperatureCharacteristicUuid;
  BLEServer *bleServer;
  static BLEService *thermometerService;
  static bool connected;
};

#endif