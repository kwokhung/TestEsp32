#ifndef Thermometer_h
#define Thermometer_h

#include <BLEServer.h>

class Thermometer : public BLEServerCallbacks
{
public:
  Thermometer(BLEServer *bleServer, char *thermometerUuid, char *temperatureUuid);

  void onConnect(BLEServer *bleServer);
  void onDisconnect(BLEServer *bleServer);
  static bool isConnected;

private:
  char *thermometerUuid;
  char *temperatureUuid;
  BLEServer *bleServer;
  BLEService *pService;
};

#endif