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

    hid = new BLEHIDDevice(bleServer);

    input = hid->inputReport(2);         // <-- input REPORTID from report map
    std::string name = "chegewara";      // <-- OPTIONAL
    hid->manufacturer()->setValue(name); // <-- OPTIONAL

    hid->pnp(0x01, 0xe502, 0xa111, 0x0210); // <-- example pnp values
    hid->hidInfo(0x00, 0x01);

    const uint8_t report[] = {
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

    hid->reportMap((uint8_t *)report, sizeof(report));
    hid->startServices();

    bleServer->getAdvertising()->setAppearance(HID_KEYBOARD);
    bleServer->getAdvertising()->addServiceUUID((uint16_t)0x1812);
    bleServer->getAdvertising()->start();

    Serial.println("SbrXXX09::Characteristic defined! Now you can read it in your phone!");
}

void SbrXXX09::loop()
{
    Serial.println("SbrXXX09::loop");

    sleepAWhileCount++;

    Input = touchRead(15); // Just test touch pin - Touch3 is T3 which is on GPIO 15.
    Serial.printf("Input: %f\n", Input);

    if (SbrXXX08::isConnected())
    {
        if (Input < 50)
        {
            /*Hid::sendKey(0x02, 0x0b);
            Hid::sendKey(0x08);
            Hid::sendKey(0x0f);
            Hid::sendKey(0x0f);
            Hid::sendKey(0x12);
            Hid::sendKey(0x28);*/

            if (sleepAWhileCount % 10 == 0)
            {
                //Hid::sendMouse(0x00, 1, 1, 0);
                uint8_t mouseData[] = {0x02, 0x00, 1, 1, 0};

                input->setValue(mouseData, sizeof(mouseData));
                input->notify();
            }

            if (sleepAWhileCount % 100 == 0)
            {
                //Hid::sendMouse(0x00, 0, 0, -1);
                uint8_t mouseData[] = {0x02, 0x00, 0, 0, -1};

                input->setValue(mouseData, sizeof(mouseData));
                input->notify();
            }
        }
        else
        {
            if (sleepAWhileCount % 10 == 0)
            {
                //Hid::sendMouse(0x00, -1, -1, 0);
                uint8_t mouseData[] = {0x02, 0x00, -1, -1, 0};

                input->setValue(mouseData, sizeof(mouseData));
                input->notify();
            }

            if (sleepAWhileCount % 100 == 0)
            {
                //Hid::sendMouse(0x00, 0, 0, 1);
                uint8_t mouseData[] = {0x02, 0x00, 0, 0, 1};

                input->setValue(mouseData, sizeof(mouseData));
                input->notify();
            }
        }
    }

    //sleepAWhile(1000);
    sleepAWhile(1);

    if (sleepAWhileCount == 1000)
    {
        sleepAWhileCount = 0;
    }
}

bool SbrXXX09::isConnected()
{
    return connected;
}

bool SbrXXX09::connected = false;
