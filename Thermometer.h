#ifndef Thermometer_h
#define Thermometer_h

#include <BLEServer.h>

class Thermometer : public BLEServerCallbacks
{
public:
  Thermometer(BLEServer *pServer, char *serviceUuid, char *characteristicUuid);

  void onConnect(BLEServer *pServer);
  void onDisconnect(BLEServer *pServer);
  static bool isConnected;
  static BLECharacteristic *pCharacteristic;

private:
  char *serviceUuid;
  char *characteristicUuid;
  BLEServer *pServer;
  BLEService *pService;
};

#endif