#include "Thermometer.h"
#include "Temperature.h"

Thermometer::Thermometer(BLEServer *pServer, char *serviceUuid, char *characteristicUuid)
    : pServer(pServer)
{
    pServer->setCallbacks(this);

    pService = pServer->createService(serviceUuid);

    new Temperature(pService, characteristicUuid);

    pService->start();
}

void Thermometer::onDisconnect(BLEServer *pServer)
{
    isConnected = false;
}

void Thermometer::onConnect(BLEServer *pServer)
{
    isConnected = true;
}

bool Thermometer::isConnected;
