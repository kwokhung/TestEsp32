#include <HardwareSerial.h>
#include <BLE2902.h>

#include "Temperature.h"

Temperature::Temperature(BLEService *pService, char *temperatureUuid)
    : pService(pService)
{
    pCharacteristic = pService->createCharacteristic(
        temperatureUuid,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_NOTIFY);
    pCharacteristic->addDescriptor(new BLE2902());
    pCharacteristic->setCallbacks(this);
}

void Temperature::onWrite(BLECharacteristic *pCharacteristic)
{
    Serial.println("*********");
    Serial.printf("New value: %d\n", getValue());
    Serial.println("*********");
}

uint8_t Temperature::getValue()
{
    value = *pCharacteristic->getValue().data();

    return value;
}

void Temperature::setValue(uint8_t newValue)
{
    value = newValue;
    pCharacteristic->setValue(&value, 1);
}

void Temperature::notify()
{
    pCharacteristic->notify();
}

uint8_t Temperature::value;
BLECharacteristic *Temperature::pCharacteristic;
