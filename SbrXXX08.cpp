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

    bleServer->getAdvertising()->setAppearance(961);
    bleServer->getAdvertising()->addServiceUUID((uint16_t)0x1812);
    bleServer->getAdvertising()->start();

    Serial.println("SbrXXX08::Characteristic defined! Now you can read it in your phone!");
}

void SbrXXX08::loop()
{
    Serial.println("SbrXXX08::loop");

    Input = touchRead(15); // Just test touch pin - Touch3 is T3 which is on GPIO 15.
    Serial.printf("Input: %f\n", Input);

    if (SbrXXX08::isConnected() && Input < 50)
    {
        Hid::sendKey(0x02, 0x0b);
        Hid::sendKey(0x08);
        Hid::sendKey(0x0f);
        Hid::sendKey(0x0f);
        Hid::sendKey(0x12);
        Hid::sendKey(0x28);
    }

    sleepAWhile(1000);
}

bool SbrXXX08::isConnected()
{
    return connected;
}

bool SbrXXX08::connected = false;