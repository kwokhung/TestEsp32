#include <HardwareSerial.h>
#include <BLEDevice.h>
#include <BLE2902.h>

#include "Thermometer.h"
#include "Temperature.h"
#include "Ble.h"

Ble::Ble(std::string name, char *serviceUuid, char *characteristicUuid)
    : name(name),
      serviceUuid(serviceUuid),
      characteristicUuid(characteristicUuid)
{
    Thermometer::isConnected = false;
    Temperature::value = 0;
}

void Ble::setup()
{
    Serial.begin(115200);

    BLEDevice::init(name);

    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new Thermometer());

    pService = pServer->createService(serviceUuid);

    pCharacteristic = pService->createCharacteristic(
        characteristicUuid,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_NOTIFY);
    pCharacteristic->addDescriptor(new BLE2902());
    pCharacteristic->setCallbacks(new Temperature());

    pService->start();

    pServer->getAdvertising()->start();

    Serial.println("Waiting a client connection...");
}

void Ble::notify()
{
    if (Thermometer::isConnected)
    {
        Serial.printf("*** Sent Value: %d ***\n", Temperature::value);

        pCharacteristic->setValue(&Temperature::value, 1);
        pCharacteristic->notify();

        Temperature::value++;
    }

    delay(1000);
}
