#ifndef Thermometer_h
#define Thermometer_h

#include <BLEServer.h>

class Thermometer : public BLEServerCallbacks
{
public:
  void onConnect(BLEServer *bleServer);
  void onDisconnect(BLEServer *bleServer);
  static void init(BLEServer *bleServer, char *thermometerServiceUuid, char *temperatureCharacteristicUuid);
  static bool isConnected();

private:
  Thermometer(BLEServer *bleServer, char *thermometerServiceUuid, char *temperatureCharacteristicUuid);

  char *thermometerServiceUuid;
  char *temperatureCharacteristicUuid;
  BLEServer *bleServer;
  static bool initted;
  static BLEService *thermometerService;
  static bool connected;
};

#endif