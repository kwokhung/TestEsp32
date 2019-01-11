#include "SbrXXX11.hpp"

void SbrXXX11::onDisconnect(BLEServer *bleServer)
{
    Serial.println("SbrXXX11::onDisconnect...");

    connected = false;
}

void SbrXXX11::onConnect(BLEServer *bleServer)
{
    Serial.println("SbrXXX11::onConnect...");

    connected = true;
}

void SbrXXX11::setup()
{
    Serial.println("SbrXXX11::setup");

    queueSize = 10;
    queue = xQueueCreate(queueSize, RW_TEST_LENGTH);

    BLEDevice::init(name);

    bleServer = BLEDevice::createServer();

    bleServer->setCallbacks(this);

    Keyboard::init(bleServer);

    bleServer->getAdvertising()->setAppearance(HID_KEYBOARD);
    bleServer->getAdvertising()->addServiceUUID((uint16_t)0x1812);
    bleServer->getAdvertising()->start();

    Serial.println("SbrXXX11::Characteristic defined! Now you can read it in your phone!");
}

void SbrXXX11::loop()
{
    //Serial.println("SbrXXX11::loop");

    if (xQueueReceive(queue, data, 0))
    {
        //Serial.printf("----Queue input: [%d] bytes ----\n", RW_TEST_LENGTH);
        displayBuffer(data, RW_TEST_LENGTH);

        if (SbrXXX11::isConnected())
        {
            Hid::sendMouse(data[0], data[1], data[2], data[3]);
        }
    }

    sleepAWhile(1000);
}

bool SbrXXX11::isConnected()
{
    return connected;
}

bool SbrXXX11::connected = false;
uint8_t *SbrXXX11::data = (uint8_t *)malloc(DATA_LENGTH);
