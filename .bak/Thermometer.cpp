#include "Thermometer.h"
#include "Temperature.h"

Thermometer::Thermometer(BLEServer *bleServer, char *thermometerServiceUuid, char *temperatureCharacteristicUuid)
    : bleServer(bleServer)
{
    bleServer->setCallbacks(this);

    thermometerService = bleServer->createService(thermometerServiceUuid);

    Temperature::init(thermometerService, temperatureCharacteristicUuid);

    thermometerService->start();
}

void Thermometer::onDisconnect(BLEServer *bleServer)
{
    connected = false;
}

void Thermometer::onConnect(BLEServer *bleServer)
{
    connected = true;
}

void Thermometer::init(BLEServer *bleServer, char *thermometerServiceUuid, char *temperatureCharacteristicUuid)
{
    if (!initted)
    {
        new Thermometer(bleServer, thermometerServiceUuid, temperatureCharacteristicUuid);
    }
}

bool Thermometer::isConnected()
{
    return connected;
}

bool Thermometer::initted = false;
bool Thermometer::connected = false;
BLEService *Thermometer::thermometerService;
