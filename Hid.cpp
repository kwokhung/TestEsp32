#include "Hid.hpp"

Hid::Hid(BLEService *keyboardService)
    : keyboardService(keyboardService)
{
    hidCharacteristic = keyboardService->createCharacteristic(
        (uint16_t)0x2a29,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_NOTIFY);
    hidCharacteristic->addDescriptor(new BLE2902());
    hidCharacteristic->setCallbacks(this);
}

void Hid::onWrite(BLECharacteristic *hidCharacteristic)
{
    Serial.println("*********");
    Serial.printf("Hid::onWrite\n");
    Serial.println("*********");
}

Hid *Hid::getSingleTon(BLEService *keyboardService)
{
    if (singleTon == NULL)
    {
        singleTon = new Hid(keyboardService);
    }

    return (singleTon);
}

void Hid::init(BLEService *keyboardService)
{
    getSingleTon(keyboardService);
}

void Hid::setValue(uint8_t *newValue, size_t length)
{
    value = newValue;
    hidCharacteristic->setValue(value, length);
}
 
void Hid::notify()
{
    hidCharacteristic->notify();
}

Hid *Hid::singleTon = NULL;
BLECharacteristic *Hid::hidCharacteristic;
uint8_t *Hid::value = 0;
