#include "SbrXXX15.hpp"

void SbrXXX15::onDisconnect(BLEServer *bleServer)
{
    Serial.println("SbrXXX15::onDisconnect...");

    connected = false;
}

void SbrXXX15::onConnect(BLEServer *bleServer)
{
    Serial.println("SbrXXX15::onConnect...");

    connected = true;
}

void SbrXXX15::setup()
{
    Serial.println("SbrXXX15::setup");

    queueSize = 10;
    queue = xQueueCreate(queueSize, sizeof(int));

    BLEDevice::init(name);

    bleServer = BLEDevice::createServer();

    bleServer->setCallbacks(this);

    Keyboard::init(bleServer);

    bleServer->getAdvertising()->setAppearance(HID_KEYBOARD);
    bleServer->getAdvertising()->addServiceUUID((uint16_t)0x1812);
    bleServer->getAdvertising()->start();

    Serial.println(name.c_str());
}

void SbrXXX15::loop()
{
    //Serial.println("SbrXXX15::loop");

    sleepAWhileCount++;

    if (SbrXXX15::isConnected())
    {
        Input = touchRead(15); // Just test touch pin - Touch3 is T3 which is on GPIO 15.
        Serial.printf("Input: %f\n", Input);

        if (Input < 50)
        {
            if (sleepAWhileCount % 10 == 0)
            {
                Hid::sendMouse(0x00, 1, 1, 0);
            }

            if (sleepAWhileCount % 100 == 0)
            {
                Hid::sendMouse(0x00, 0, 0, -1);
            }

            if (sleepAWhileCount % 1000 == 0)
            {
                //Hid::sendKey((uint8_t)KEY_SHIFT, 0x0b);
                //Hid::sendKey(0x08);
                //Hid::sendKey(0x0f);
                //Hid::sendKey(0x0f);
                //Hid::sendKey(0x12);
                Hid::sendKey(0x11);
                Hid::sendKey(0x0c);
                Hid::sendKey(0x2c);
                Hid::sendKey(0x0b);
                Hid::sendKey(0x04);
                Hid::sendKey(0x12);
                Hid::sendKey(0x2c);
                Hid::sendKey(0x10);
                Hid::sendKey(0x04);
                Hid::sendKey(0x2c);
                Hid::sendKey(0x28);
            }
        }
        else
        {
            if (sleepAWhileCount % 10 == 0)
            {
                Hid::sendMouse(0x00, -1, -1, 0);
            }

            if (sleepAWhileCount % 100 == 0)
            {
                Hid::sendMouse(0x00, 0, 0, 1);
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

bool SbrXXX15::isConnected()
{
    return connected;
}

uint32_t SbrXXX15::sleepAWhileCount = 0;
bool SbrXXX15::connected = false;
int8_t SbrXXX15::x = 0;
int8_t SbrXXX15::y = 0;
