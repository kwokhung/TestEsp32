#include "Thermometer.h"
#include "Temperature.h"

Thermometer::Thermometer(BLEServer *bleServer, char *thermometerServiceUuid, char *temperatureCharacteristicUuid)
    : bleServer(bleServer)
{
    bleServer->setCallbacks(this);

    thermometerService = bleServer->createService(thermometerServiceUuid);

    //new Temperature(thermometerService, temperatureCharacteristicUuid);
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

bool Thermometer::isConnected()
{
    return connected;
}

bool Thermometer::connected = false;
BLEService *Thermometer::thermometerService;
