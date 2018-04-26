#include "Hid.hpp"

Hid::Hid(BLEService *keyboardService, BLEService *keyboardService1)
    : keyboardService(keyboardService),
      keyboardService1(keyboardService1)
{
    manufacturer = keyboardService->createCharacteristic(
        (uint16_t)0x2a29,
        BLECharacteristic::PROPERTY_READ);
    std::string name = "espressif";
    manufacturer->setValue(name);

    pnpIDChar = keyboardService->createCharacteristic(
        (uint16_t)0x2a50,
        BLECharacteristic::PROPERTY_READ);
    const uint8_t pnp[] = {0x01, 0xe5, 0x02, 0xcd, 0xab, 0x01, 0x00};
    pnpIDChar->setValue((uint8_t *)pnp, sizeof(pnp));

    hidInfoChar = keyboardService1->createCharacteristic(
        (uint16_t)0x2a4a,
        BLECharacteristic::PROPERTY_READ);
    const uint8_t val1[] = {0x00, 0x01, 0x00, 0x02};
    hidInfoChar->setValue((uint8_t *)val1, 4);

    reportMapChar = keyboardService1->createCharacteristic(
        (uint16_t)0x2a4b,
        BLECharacteristic::PROPERTY_READ);
    const uint8_t val2[] = {
        0x05, 0x01, 0x09, 0x06, 0xA1, 0x01, 0x05, 0x07,
        0x19, 0xE0, 0x29, 0xE7, 0x15, 0x00, 0x25, 0x01,
        0x75, 0x01, 0x95, 0x08, 0x81, 0x02, 0x95, 0x01,
        0x75, 0x08, 0x81, 0x01, 0x95, 0x05, 0x75, 0x01,
        0x05, 0x08, 0x19, 0x01, 0x29, 0x05, 0x91, 0x02,
        0x95, 0x01, 0x75, 0x03, 0x91, 0x01, 0x95, 0x06,
        0x75, 0x08, 0x15, 0x00, 0x25, 0x65, 0x05, 0x07,
        0x19, 0x00, 0x29, 0x65, 0x81, 0x00, 0xC0};
    reportMapChar->setValue((uint8_t *)val2, sizeof(val2));

    hidControlChar = keyboardService1->createCharacteristic(
        (uint16_t)0x2a4c,
        BLECharacteristic::PROPERTY_WRITE_NR);

    reportChar1 = keyboardService1->createCharacteristic(
        (uint16_t)0x2a4d,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_NOTIFY);
    BLEDescriptor *desc1 = new BLEDescriptor(BLEUUID((uint16_t)0x2908));
    const uint8_t desc1_val[] = {0x01, 0};
    desc1->setValue((uint8_t *)desc1_val, 1);
    reportChar1->addDescriptor(desc1);
    reportChar1->addDescriptor(new BLE2902());

    reportChar2 = keyboardService1->createCharacteristic(
        (uint16_t)0x2a4d,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE);
    BLEDescriptor *desc2 = new BLEDescriptor(BLEUUID((uint16_t)0x2908));
    const uint8_t desc2_val[] = {0x02, 0};
    desc2->setValue((uint8_t *)desc2_val, 1);
    reportChar2->addDescriptor(desc2);

    reportChar3 = keyboardService1->createCharacteristic(
        (uint16_t)0x2a4d,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_WRITE_NR);
    BLEDescriptor *desc3 = new BLEDescriptor(BLEUUID((uint16_t)0x2908));
    const uint8_t desc3_val[] = {0x03, 0};
    desc3->setValue((uint8_t *)desc3_val, 1);
    reportChar3->addDescriptor(desc3);

    protocolModeChar = keyboardService1->createCharacteristic(
        (uint16_t)0x2a4e,
        BLECharacteristic::PROPERTY_WRITE_NR);

    bootInputChar = keyboardService1->createCharacteristic(
        (uint16_t)0x2a22,
        BLECharacteristic::PROPERTY_NOTIFY);
    bootInputChar->addDescriptor(new BLE2902());

    bootOutputChar = keyboardService1->createCharacteristic(
        (uint16_t)0x2a32,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_WRITE_NR);
}

Hid *Hid::getSingleTon(BLEService *keyboardService, BLEService *keyboardService1)
{
    if (singleTon == NULL)
    {
        singleTon = new Hid(keyboardService, keyboardService1);
    }

    return (singleTon);
}

void Hid::init(BLEService *keyboardService, BLEService *keyboardService1)
{
    getSingleTon(keyboardService, keyboardService1);
}

void Hid::setValue(uint8_t *newValue, size_t length)
{
    value = newValue;
    reportChar1->setValue(value, length);
}

void Hid::notify()
{
    reportChar1->notify();
}

Hid *Hid::singleTon = NULL;
BLECharacteristic *Hid::manufacturer;
BLECharacteristic *Hid::pnpIDChar;
BLECharacteristic *Hid::hidInfoChar;
BLECharacteristic *Hid::reportMapChar;
BLECharacteristic *Hid::hidControlChar;
BLECharacteristic *Hid::reportChar1;
BLECharacteristic *Hid::reportChar2;
BLECharacteristic *Hid::reportChar3;
BLECharacteristic *Hid::protocolModeChar;
BLECharacteristic *Hid::bootInputChar;
BLECharacteristic *Hid::bootOutputChar;
uint8_t *Hid::value = 0;
