#include "Thermometer.h"
#include "Temperature.h"

Thermometer::Thermometer(BLEServer *bleServer, char *thermometerUuid, char *temperatureUuid)
    : bleServer(bleServer)
{
    bleServer->setCallbacks(this);

    pService = bleServer->createService(thermometerUuid);

    new Temperature(pService, temperatureUuid);

    pService->start();
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
