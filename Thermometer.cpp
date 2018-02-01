#include "Thermometer.h"

void Thermometer::onDisconnect(BLEServer *pServer)
{
    isConnected = false;
}

void Thermometer::onConnect(BLEServer *pServer)
{
    isConnected = true;
}

bool Thermometer::isConnected;
