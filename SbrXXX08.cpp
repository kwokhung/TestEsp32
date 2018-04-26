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
        Serial.printf("Hid::Sent Value: %d\n", Hid::getValue());

        Hid::notify();

        Hid::setValue(Hid::getValue() + 1);
    }

    sleepAWhile(1000);
}

bool SbrXXX08::isConnected()
{
    return connected;
}

bool SbrXXX08::connected = false;