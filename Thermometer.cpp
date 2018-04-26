#include "Thermometer.hpp"

Thermometer::Thermometer(BLEServer *bleServer, char *thermometerServiceUuid, char *temperatureCharacteristicUuid)
    : bleServer(bleServer)
{
    thermometerService = bleServer->createService(thermometerServiceUuid);

    Temperature::init(thermometerService, temperatureCharacteristicUuid);

    thermometerService->start();
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
}

Thermometer *Thermometer::singleTon = NULL;
BLEService *Thermometer::thermometerService;
