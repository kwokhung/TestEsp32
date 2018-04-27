#include "Hid.hpp"

Hid::Hid(BLEService *deviceInformationService, BLEService *humanInterfaceDeviceService)
    : deviceInformationService(deviceInformationService),
      humanInterfaceDeviceService(humanInterfaceDeviceService)
{
    pnpId = deviceInformationService->createCharacteristic(
        (uint16_t)0x2a50,
        BLECharacteristic::PROPERTY_READ);

    const uint8_t pnp[] = {0x01, 0xe5, 0x02, 0xcd, 0xab, 0x01, 0x00};

    pnpId->setValue((uint8_t *)pnp, sizeof(pnp));

    manufacturerNameString = deviceInformationService->createCharacteristic(
        (uint16_t)0x2a29,
        BLECharacteristic::PROPERTY_READ);

    std::string name = "espressif";

    manufacturerNameString->setValue(name);

    hidInformation = humanInterfaceDeviceService->createCharacteristic(
        (uint16_t)0x2a4a,
        BLECharacteristic::PROPERTY_READ);

    const uint8_t val1[] = {0x00, 0x01, 0x00, 0x02};

    hidInformation->setValue((uint8_t *)val1, sizeof(val1));

    reportMap = humanInterfaceDeviceService->createCharacteristic(
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

    reportMap->setValue((uint8_t *)val2, sizeof(val2));

    hidControlPoint = humanInterfaceDeviceService->createCharacteristic(
        (uint16_t)0x2a4c,
        BLECharacteristic::PROPERTY_WRITE_NR);

    report1 = humanInterfaceDeviceService->createCharacteristic(
        (uint16_t)0x2a4d,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_NOTIFY);

    report1->addDescriptor(new BLE2902());

    BLEDescriptor *reportReference1 = new BLEDescriptor(BLEUUID((uint16_t)0x2908));
    const uint8_t reportReference1_val[] = {0x01};
    reportReference1->setValue((uint8_t *)reportReference1_val, sizeof(reportReference1_val));

    report1->addDescriptor(reportReference1);

    report2 = humanInterfaceDeviceService->createCharacteristic(
        (uint16_t)0x2a4d,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE);

    BLEDescriptor *reportReference2 = new BLEDescriptor(BLEUUID((uint16_t)0x2908));
    const uint8_t reportReference2_val[] = {0x02};
    reportReference2->setValue((uint8_t *)reportReference2_val, sizeof(reportReference2_val));

    report2->addDescriptor(reportReference2);

    report3 = humanInterfaceDeviceService->createCharacteristic(
        (uint16_t)0x2a4d,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_WRITE_NR);

    BLEDescriptor *reportReference3 = new BLEDescriptor(BLEUUID((uint16_t)0x2908));
    const uint8_t reportReference3_val[] = {0x03};
    reportReference3->setValue((uint8_t *)reportReference3_val, sizeof(reportReference3_val));

    report3->addDescriptor(reportReference3);

    protocolMode = humanInterfaceDeviceService->createCharacteristic(
        (uint16_t)0x2a4e,
        BLECharacteristic::PROPERTY_WRITE_NR);

    bootKeyboradInputReport = humanInterfaceDeviceService->createCharacteristic(
        (uint16_t)0x2a22,
        BLECharacteristic::PROPERTY_NOTIFY);

    bootKeyboradInputReport->addDescriptor(new BLE2902());

    bootKeyboardOutputReport = humanInterfaceDeviceService->createCharacteristic(
        (uint16_t)0x2a32,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_WRITE_NR);
}

Hid *Hid::getSingleTon(BLEService *deviceInformationService, BLEService *humanInterfaceDeviceService)
{
    if (singleTon == NULL)
    {
        singleTon = new Hid(deviceInformationService, humanInterfaceDeviceService);
    }

    return (singleTon);
}

void Hid::init(BLEService *deviceInformationService, BLEService *humanInterfaceDeviceService)
{
    getSingleTon(deviceInformationService, humanInterfaceDeviceService);
}

void Hid::setValue(uint8_t *newValue, size_t length)
{
    value = newValue;
    report1->setValue(value, length);
}

void Hid::notify()
{
    report1->notify();
}

Hid *Hid::singleTon = NULL;
BLECharacteristic *Hid::pnpId;
BLECharacteristic *Hid::manufacturerNameString;
BLECharacteristic *Hid::hidInformation;
BLECharacteristic *Hid::reportMap;
BLECharacteristic *Hid::hidControlPoint;
BLECharacteristic *Hid::report1;
BLECharacteristic *Hid::report2;
BLECharacteristic *Hid::report3;
BLECharacteristic *Hid::protocolMode;
BLECharacteristic *Hid::bootKeyboradInputReport;
BLECharacteristic *Hid::bootKeyboardOutputReport;
uint8_t *Hid::value = 0;
