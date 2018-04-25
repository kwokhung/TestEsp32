#include "Temperature.hpp"

Temperature::Temperature(BLEService *thermometerService, char *temperatureCharacteristicUuid)
    : thermometerService(thermometerService)
{
    temperatureCharacteristic = thermometerService->createCharacteristic(
        temperatureCharacteristicUuid,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_NOTIFY);
    temperatureCharacteristic->addDescriptor(new BLE2902());
    temperatureCharacteristic->setCallbacks(this);
}

void Temperature::onWrite(BLECharacteristic *temperatureCharacteristic)
{
    Serial.println("*********");
    Serial.printf("New value: %d\n", getValue());
    Serial.println("*********");
}

Temperature *Temperature::getSingleTon(BLEService *thermometerService, char *temperatureCharacteristicUuid)
{
    if (singleTon == NULL)
    {
        singleTon = new Temperature(thermometerService, temperatureCharacteristicUuid);
    }

    return (singleTon);
}

void Temperature::init(BLEService *thermometerService, char *temperatureCharacteristicUuid)
{
    getSingleTon(thermometerService, temperatureCharacteristicUuid);
}

uint8_t Temperature::getValue()
{
    value = *temperatureCharacteristic->getValue().data();

    return value;
}

void Temperature::setValue(uint8_t newValue)
{
    value = newValue;
    temperatureCharacteristic->setValue(&value, 1);
}

void Temperature::notify()
{
    temperatureCharacteristic->notify();
}

Temperature *Temperature::singleTon = NULL;
BLECharacteristic *Temperature::temperatureCharacteristic;
uint8_t Temperature::value = 0;
