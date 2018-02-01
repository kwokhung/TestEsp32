#include <HardwareSerial.h>
#include <BLE2902.h>

#include "Temperature.h"

Temperature::Temperature(BLEService *pService, char *characteristicUuid)
    : pService(pService)
{
    pCharacteristic = pService->createCharacteristic(
        characteristicUuid,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_NOTIFY);
    pCharacteristic->addDescriptor(new BLE2902());
    pCharacteristic->setCallbacks(this);
}

void Temperature::onWrite(BLECharacteristic *pCharacteristic)
{
    value = *pCharacteristic->getValue().data();
    Serial.println("*********");
    Serial.printf("New value: %d\n", value);
    Serial.println("*********");
}

uint8_t Temperature::value;
BLECharacteristic *Temperature::pCharacteristic;
