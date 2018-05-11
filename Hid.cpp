#include "Hid.hpp"

Hid::Hid(BLEService *deviceInformationService, BLEService *humanInterfaceDeviceService, BLEService *batteryService)
    : deviceInformationService(deviceInformationService),
      humanInterfaceDeviceService(humanInterfaceDeviceService),
      batteryService(batteryService)
{
    pnpId = deviceInformationService->createCharacteristic(
        (uint16_t)0x2a50,
        BLECharacteristic::PROPERTY_READ);

    //const uint8_t pnpIdValue[] = {0x01, 0xe5, 0x02, 0xcd, 0xab, 0x01, 0x00};
    const uint8_t pnpIdValue[] = {0x01, (uint8_t)(0xe502 >> 8), (uint8_t)0xe502, (uint8_t)(0xa111 >> 8), (uint8_t)0xa111, (uint8_t)(0x0210 >> 8), (uint8_t)0x0210};

    pnpId->setValue((uint8_t *)pnpIdValue, sizeof(pnpIdValue));

    manufacturerNameString = deviceInformationService->createCharacteristic(
        (uint16_t)0x2a29,
        BLECharacteristic::PROPERTY_READ);

    //const std::string name = "espressif";
    const std::string name = "chegewara";

    manufacturerNameString->setValue(name);

    hidInformation = humanInterfaceDeviceService->createCharacteristic(
        (uint16_t)0x2a4a,
        BLECharacteristic::PROPERTY_READ);

    //const uint8_t hidInformationValue[] = {0x00, 0x01, 0x00, 0x02};
    const uint8_t hidInformationValue[] = {0x11, 0x01, 0x00, 0x01};

    hidInformation->setValue((uint8_t *)hidInformationValue, sizeof(hidInformationValue));

    reportMap = humanInterfaceDeviceService->createCharacteristic(
        (uint16_t)0x2a4b,
        BLECharacteristic::PROPERTY_READ);

    const uint8_t reportMapValue[] = {
        /**/
        0x05, 0x01, // USAGE_PAGE (Generic Desktop)
        0x09, 0x06, // USAGE (Keyboard)
        0xa1, 0x01, // COLLECTION (Application)
        0x85, 0x01, //   REPORT_ID (1)
        0x05, 0x07, //   USAGE_PAGE (Keyboard)
        0x19, 0xe0, //   USAGE_MINIMUM (Keyboard LeftControl)
        0x29, 0xe7, //   USAGE_MAXIMUM (Keyboard Right GUI)
        0x15, 0x00, //   LOGICAL_MINIMUM (0)
        0x25, 0x01, //   LOGICAL_MAXIMUM (1)
        0x75, 0x01, //   REPORT_SIZE (1)
        0x95, 0x08, //   REPORT_COUNT (8)
        0x81, 0x02, //   INPUT (Data,Var,Abs)
        0x95, 0x01, //   REPORT_COUNT (1)
        0x75, 0x08, //   REPORT_SIZE (8)
        0x81, 0x03, //   INPUT (Cnst,Var,Abs)
        0x95, 0x06, //   REPORT_COUNT (6)
        0x75, 0x08, //   REPORT_SIZE (8)
        0x15, 0x00, //   LOGICAL_MINIMUM (0)
        0x25, 0x65, //   LOGICAL_MAXIMUM (101)
        0x05, 0x07, //   USAGE_PAGE (Keyboard)
        0x19, 0x00, //   USAGE_MINIMUM (Reserved (no event indicated))
        0x29, 0x65, //   USAGE_MAXIMUM (Keyboard Application)
        0x81, 0x00, //   INPUT (Data,Ary,Abs)
        0xc0,       // END_COLLECTION
        /**/
        0x05, 0x01, // USAGE_PAGE (Generic Desktop)
        0x09, 0x02, // USAGE (Mouse)
        0xa1, 0x01, // COLLECTION (Application)
        0x09, 0x01, //   USAGE (Pointer)
        0xa1, 0x00, //   COLLECTION (Physical)
        0x85, 0x02, //     REPORT_ID (2)
        0x05, 0x09, //     USAGE_PAGE (Button)
        0x19, 0x01, //     USAGE_MINIMUM (Button 1)
        0x29, 0x03, //     USAGE_MAXIMUM (Button 3)
        0x15, 0x00, //     LOGICAL_MINIMUM (0)
        0x25, 0x01, //     LOGICAL_MAXIMUM (1)
        0x95, 0x03, //     REPORT_COUNT (3)
        0x75, 0x01, //     REPORT_SIZE (1)
        0x81, 0x02, //     INPUT (Data,Var,Abs)
        0x95, 0x01, //     REPORT_COUNT (1)
        0x75, 0x05, //     REPORT_SIZE (5)
        0x81, 0x03, //     INPUT (Cnst,Var,Abs)
        0x05, 0x01, //     USAGE_PAGE (Generic Desktop)
        0x09, 0x30, //     USAGE (X)
        0x09, 0x31, //     USAGE (Y)
        0x09, 0x38, //     USAGE (Wheel)
        0x15, 0x81, //     LOGICAL_MINIMUM (-127)
        0x25, 0x7f, //     LOGICAL_MAXIMUM (127)
        0x75, 0x08, //     REPORT_SIZE (8)
        0x95, 0x03, //     REPORT_COUNT (3)
        0x81, 0x06, //     INPUT (Data,Var,Rel)
        0xc0,       //   END_COLLECTION
        0xc0,       // END_COLLECTION
    };

    reportMap->setValue((uint8_t *)reportMapValue, sizeof(reportMapValue));

    hidControlPoint = humanInterfaceDeviceService->createCharacteristic(
        (uint16_t)0x2a4c,
        BLECharacteristic::PROPERTY_WRITE_NR);

    mouseInputReport = humanInterfaceDeviceService->createCharacteristic(
        (uint16_t)0x2a4d,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_NOTIFY);

    mouseInputReport->addDescriptor(new BLE2902());

    BLEDescriptor *mouseInputReportReference = new BLEDescriptor(BLEUUID((uint16_t)0x2908));
    const uint8_t mouseInputReportReferenceValue[] = {0x02, 0x01};
    mouseInputReportReference->setValue((uint8_t *)mouseInputReportReferenceValue, sizeof(mouseInputReportReferenceValue));

    mouseInputReport->addDescriptor(mouseInputReportReference);

    keyboardInputReport = humanInterfaceDeviceService->createCharacteristic(
        (uint16_t)0x2a4d,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_NOTIFY);

    keyboardInputReport->addDescriptor(new BLE2902());

    BLEDescriptor *keyboardInputReportReference = new BLEDescriptor(BLEUUID((uint16_t)0x2908));
    const uint8_t keyboardInputReportReferenceValue[] = {0x01, 0x01};
    keyboardInputReportReference->setValue((uint8_t *)keyboardInputReportReferenceValue, sizeof(keyboardInputReportReferenceValue));

    keyboardInputReport->addDescriptor(keyboardInputReportReference);
/*
    outputReport = humanInterfaceDeviceService->createCharacteristic(
        (uint16_t)0x2a4d,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE);

    BLEDescriptor *outputReportReference = new BLEDescriptor(BLEUUID((uint16_t)0x2908));
    const uint8_t outputReportReferenceValue[] = {0x02};
    outputReportReference->setValue((uint8_t *)outputReportReferenceValue, sizeof(outputReportReferenceValue));

    outputReport->addDescriptor(outputReportReference);

    featureReport = humanInterfaceDeviceService->createCharacteristic(
        (uint16_t)0x2a4d,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_WRITE_NR);

    BLEDescriptor *featureReportReference = new BLEDescriptor(BLEUUID((uint16_t)0x2908));
    const uint8_t featureReportReferenceValue[] = {0x03};
    featureReportReference->setValue((uint8_t *)featureReportReferenceValue, sizeof(featureReportReferenceValue));

    featureReport->addDescriptor(featureReportReference);
*/
    protocolMode = humanInterfaceDeviceService->createCharacteristic(
        (uint16_t)0x2a4e,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE_NR);

    const uint8_t protocolModeValue[] = {0x01};

    protocolMode->setValue((uint8_t *)protocolModeValue, sizeof(hidInformationValue));
/*
    bootKeyboardInputReport = humanInterfaceDeviceService->createCharacteristic(
        (uint16_t)0x2a22,
        BLECharacteristic::PROPERTY_NOTIFY);

    bootKeyboardInputReport->addDescriptor(new BLE2902());

    bootKeyboardOutputReport = humanInterfaceDeviceService->createCharacteristic(
        (uint16_t)0x2a32,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_WRITE_NR);

    bootMouseInputReport = humanInterfaceDeviceService->createCharacteristic(
        (uint16_t)0x2a33,
        BLECharacteristic::PROPERTY_NOTIFY);

    bootMouseInputReport->addDescriptor(new BLE2902());
*/
    batteryLevel = batteryService->createCharacteristic(
        (uint16_t)0x2a19,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_NOTIFY);

    batteryLevel->addDescriptor(new BLE2902());

    BLE2904 *batteryLevelDescriptor = new BLE2904();
    batteryLevelDescriptor->setFormat(BLE2904::FORMAT_UINT8);
    batteryLevelDescriptor->setNamespace(1);
    batteryLevelDescriptor->setUnit(0x27ad);

    batteryLevel->addDescriptor(batteryLevelDescriptor);
}

Hid *Hid::getSingleTon(BLEService *deviceInformationService, BLEService *humanInterfaceDeviceService, BLEService *batteryService)
{
    if (singleTon == NULL)
    {
        singleTon = new Hid(deviceInformationService, humanInterfaceDeviceService, batteryService);
    }

    return (singleTon);
}

void Hid::init(BLEService *deviceInformationService, BLEService *humanInterfaceDeviceService, BLEService *batteryService)
{
    getSingleTon(deviceInformationService, humanInterfaceDeviceService, batteryService);
}

void Hid::setValue(BLECharacteristic *inputReport, uint8_t *value, size_t length)
{
    inputReport->setValue(value, length);
}

void Hid::notify(BLECharacteristic *inputReport)
{
    inputReport->notify();
}

void Hid::sendKey(uint8_t modifier, uint8_t key)
{
    Serial.printf("modifier: %d\n", modifier);
    Serial.printf("key: %d\n", key);

    uint8_t keyPressed[] = {/*0x01, */modifier, 0x00, key, 0x00, 0x00, 0x00, 0x00, 0x00};

    Hid::setValue(keyboardInputReport, keyPressed, sizeof(keyPressed));
    Hid::notify(keyboardInputReport);

    uint8_t keyReleased[] = {/*0x01, */0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    Hid::setValue(keyboardInputReport, keyReleased, sizeof(keyReleased));
    Hid::notify(keyboardInputReport);
}

void Hid::sendKey(uint8_t key)
{
    sendKey(0x00, key);
}

void Hid::sendMouse(uint8_t buttons, int8_t x, int8_t y, int8_t wheel)
{
    Serial.printf("buttons: %d\n", buttons);
    Serial.printf("x: %d\n", x);
    Serial.printf("y: %d\n", y);
    Serial.printf("wheel: %d\n", wheel);

    uint8_t mouseData[] = {/*0x02, */buttons, x, y, wheel};

    Hid::setValue(mouseInputReport, mouseData, sizeof(mouseData));
    Hid::notify(mouseInputReport);
}

Hid *Hid::singleTon = NULL;
BLECharacteristic *Hid::pnpId;
BLECharacteristic *Hid::manufacturerNameString;
BLECharacteristic *Hid::hidInformation;
BLECharacteristic *Hid::reportMap;
BLECharacteristic *Hid::hidControlPoint;
BLECharacteristic *Hid::mouseInputReport;
BLECharacteristic *Hid::keyboardInputReport;
BLECharacteristic *Hid::outputReport;
BLECharacteristic *Hid::featureReport;
BLECharacteristic *Hid::protocolMode;
BLECharacteristic *Hid::bootKeyboardInputReport;
BLECharacteristic *Hid::bootKeyboardOutputReport;
BLECharacteristic *Hid::bootMouseInputReport;
BLECharacteristic *Hid::batteryLevel;
