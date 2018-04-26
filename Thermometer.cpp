#include "Thermometer.hpp"

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
    Serial.println("Thermometer: onDisconnect...");

    connected = false;
}

void Thermometer::onConnect(BLEServer *bleServer)
{
    Serial.println("Thermometer: onConnect...");

    connected = true;
}

Thermometer *Thermometer::getSingleTon(BLEServer *bleServer, char *thermometerServiceUuid, char *temperatureCharacteristicUuid)
{
    if (singleTon == NULL)
    {
        singleTon = new Thermometer(bleServer, thermometerServiceUuid, temperatureCharacteristicUuid);
    }

    return (singleTon);
}

void Thermometer::init(BLEServer *bleServer, char *thermometerServiceUuid, char *temperatureCharacteristicUuid)
{
    getSingleTon(bleServer, thermometerServiceUuid, temperatureCharacteristicUuid);

    bleServer->getAdvertising()->start();

    Serial.println("Thermometer: Waiting a client connection...");
}

bool Thermometer::isConnected()
{
    return connected;
}

Thermometer *Thermometer::singleTon = NULL;
BLEService *Thermometer::thermometerService;
bool Thermometer::connected = false;
