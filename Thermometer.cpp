#include <BLE2902.h>

#include "Thermometer.h"
#include "Temperature.h"

Thermometer::Thermometer(BLEServer *pServer, char *serviceUuid, char *characteristicUuid)
    : pServer(pServer)
{
    pServer->setCallbacks(this);

    pService = pServer->createService(serviceUuid);

    pCharacteristic = pService->createCharacteristic(
        characteristicUuid,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_NOTIFY);
    pCharacteristic->addDescriptor(new BLE2902());
    pCharacteristic->setCallbacks(new Temperature());

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
BLECharacteristic *Thermometer::pCharacteristic;
