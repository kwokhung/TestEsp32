#ifndef Thermometer_h
#define Thermometer_h

#include <BLEServer.h>

class Thermometer : public BLEServerCallbacks
{
  public:
    void onConnect(BLEServer *pServer);
    void onDisconnect(BLEServer *pServer);
    static bool isConnected;
};

#endif