#include "SbrXXX08.hpp"

void SbrXXX08::onDisconnect(BLEServer *bleServer)
{
    Serial.println("SbrXXX08::onDisconnect...");

    connected = false;
}

void SbrXXX08::onConnect(BLEServer *bleServer)
{
    Serial.println("SbrXXX08::onConnect...");

    connected = true;
}

void SbrXXX08::setup()
{
    Serial.println("SbrXXX08::setup");

    queueSize = 10;
    queue = xQueueCreate(queueSize, sizeof(int));

    BLEDevice::init(name);

    bleServer = BLEDevice::createServer();

    bleServer->setCallbacks(this);

    Keyboard::init(bleServer);

    bleServer->getAdvertising()->setAppearance(HID_KEYBOARD);
    bleServer->getAdvertising()->addServiceUUID((uint16_t)0x1812);
    bleServer->getAdvertising()->start();

    Serial.println("SbrXXX08::Characteristic defined! Now you can read it in your phone!");
}

void SbrXXX08::loop()
{
    Serial.println("SbrXXX08::loop");

    sleepAWhileCount++;

    Input = touchRead(15); // Just test touch pin - Touch3 is T3 which is on GPIO 15.
    Serial.printf("Input: %f\n", Input);

    if (SbrXXX08::isConnected())
    {
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
                Hid::sendKey(0x02, 0x0b);
                Hid::sendKey(0x08);
                Hid::sendKey(0x0f);
                Hid::sendKey(0x0f);
                Hid::sendKey(0x12);
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

bool SbrXXX08::isConnected()
{
    return connected;
}

uint32_t SbrXXX08::sleepAWhileCount = 0;
bool SbrXXX08::connected = false;
int8_t SbrXXX08::x = 0;
int8_t SbrXXX08::y = 0;
