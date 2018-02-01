#include "Thermometer.h"
#include "Temperature.h"

Thermometer::Thermometer(BLEServer *bleServer, char *thermometerServiceUuid, char *temperatureCharacteristicUuid)
    : bleServer(bleServer)
{
    bleServer->setCallbacks(this);

    thermometerService = bleServer->createService(thermometerServiceUuid);

    new Temperature(thermometerService, temperatureCharacteristicUuid);

    thermometerService->start();
}

void Thermometer::onDisconnect(BLEServer *bleServer)
{
    isConnected = false;
}

void Thermometer::onConnect(BLEServer *bleServer)
{
    isConnected = true;
}

bool Thermometer::isConnected;
