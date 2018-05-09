#include "SbrXXX09.hpp"

void SbrXXX09::onDisconnect(BLEServer *bleServer)
{
    Serial.println("SbrXXX09::onDisconnect...");

    connected = false;
}

void SbrXXX09::onConnect(BLEServer *bleServer)
{
    Serial.println("SbrXXX09::onConnect...");

    connected = true;
}

void SbrXXX09::setup()
{
    Serial.println("SbrXXX09::setup");

    queueSize = 10;
    queue = xQueueCreate(queueSize, sizeof(int));

    BLEDevice::init(name);

    bleServer = BLEDevice::createServer();

    bleServer->setCallbacks(this);

    BLEHIDDevice *hid;
    hid = new BLEHIDDevice(bleServer);

    BLECharacteristic *input = hid->inputReport(1); // <-- input REPORTID from report map
    std::string name = "chegewara";                 // <-- OPTIONAL
    hid->manufacturer()->setValue(name);            // <-- OPTIONAL

    hid->pnp(0x01, 0xe502, 0xa111, 0x0210); // <-- example pnp values
    hid->hidInfo(0x00, 0x01);

    const uint8_t report[] = {

        0x05, 0x01,       // Usage Page (Generic Desktop Ctrls)
        0x09, 0x02,       // Usage (Mouse)
        0xA1, 0x01,       // Collection (Application)
        0x85, 0x01,       //   Report ID (1)
        0x09, 0x01,       //   Usage (Pointer)
        0xA1, 0x00,       //   Collection (Physical)
        0x05, 0x09,       //     Usage Page (Button)
        0x19, 0x01,       //     Usage Minimum (0x01)
        0x29, 0x03,       //     Usage Maximum (0x03)
        0x25, 0x01,       //     Logical Maximum (1)
        0x75, 0x01,       //     Report Size (1)
        0x95, 0x03,       //     Report Count (3)
        0x81, 0x02,       //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
        0x75, 0x05,       //     Report Size (5)
        0x95, 0x01,       //     Report Count (1)
        0x81, 0x01,       //     Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
        0x05, 0x01,       //     Usage Page (Generic Desktop Ctrls)
        0x09, 0x30,       //     Usage (X)
        0x26, 0x05, 0x56, //     Logical Maximum (127)
        0x09, 0x31,       //     Usage (Y)
        0x26, 0x03, 0x00, //     Logical Maximum (127)
        0x09, 0x38,       //     Usage (Wheel)
        0x15, 0x00,       //     Logical Minimum (-127)
        0x25, 0x7f,       //     Logical Maximum (127)
        0x75, 0x08,       //     Report Size (8)
        0x95, 0x03,       //     Report Count (3)
        0x81, 0x02,       //     Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
        0xC0,             //   End Collection
        0xC0,             // End Collection

        // 52 bytes
    };

    hid->reportMap((uint8_t *)report, sizeof(report));
    hid->startServices();

    bleServer->getAdvertising()->setAppearance(961);
    bleServer->getAdvertising()->addServiceUUID((uint16_t)0x1812);
    bleServer->getAdvertising()->start();

    Serial.println("SbrXXX09::Characteristic defined! Now you can read it in your phone!");
}

void SbrXXX09::loop()
{
    Serial.println("SbrXXX09::loop");

    sleepAWhile(1000);
}

bool SbrXXX09::isConnected()
{
    return connected;
}

bool SbrXXX09::connected = false;
