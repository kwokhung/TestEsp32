#include "SbrXXX07.hpp"

void SbrXXX07::onDisconnect(BLEServer *bleServer)
{
    Serial.println("SbrXXX07::onDisconnect...");

    connected = false;
}

void SbrXXX07::onConnect(BLEServer *bleServer)
{
    Serial.println("SbrXXX07::onConnect...");

    connected = true;
}

void SbrXXX07::setup()
{
    Serial.println("SbrXXX07::setup");

    queueSize = 10;
    queue = xQueueCreate(queueSize, sizeof(int));

    BLEDevice::init(name);

    bleServer = BLEDevice::createServer();

    bleServer->setCallbacks(this);

    Thermometer::init(bleServer, "4fafc201-1fb5-459e-8fcc-c5c9c331914b", "beb5483e-36e1-4688-b7f5-ea07361b26a8");

    bleServer->getAdvertising()->start();

    Serial.println("SbrXXX07::Waiting a client connection...");
}

void SbrXXX07::loop()
{
    Serial.println("SbrXXX07::loop");

    if (SbrXXX07::isConnected())
    {
        Serial.printf("Temperature::Sent Value: %d\n", Temperature::getValue());

        Temperature::notify();

        Temperature::setValue(Temperature::getValue() + 1);
    }

    sleepAWhile(1000);
}

bool SbrXXX07::isConnected()
{
    return connected;
}

bool SbrXXX07::connected = false;
