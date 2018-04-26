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

    if (SbrXXX08::isConnected())
    {
        uint8_t v[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
        uint8_t a[] = {0x0, 0x0, random(0x04, 0x26), 0x0, 0x0, 0x0, 0x0, 0x0};

        Hid::setValue(a, sizeof(a));

        Hid::notify();

        Hid::setValue(v, sizeof(v));

        Hid::notify();
    }

    sleepAWhile(1000);
}

bool SbrXXX08::isConnected()
{
    return connected;
}

bool SbrXXX08::connected = false;