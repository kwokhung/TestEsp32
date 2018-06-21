#include "SbrXXX13.hpp"

void SbrXXX13::onDisconnect(BLEServer *bleServer)
{
    Serial.println("SbrXXX13::onDisconnect...");

    connected = false;
}

void SbrXXX13::onConnect(BLEServer *bleServer)
{
    Serial.println("SbrXXX13::onConnect...");

    connected = true;
}

void SbrXXX13::setup()
{
    Serial.println("SbrXXX13::setup");

    queueSize = 10;
    queue = xQueueCreate(queueSize, sizeof(int));

    BLEDevice::init(name);

    bleServer = BLEDevice::createServer();

    bleServer->setCallbacks(this);

    Keyboard::init(bleServer);

    bleServer->getAdvertising()->setAppearance(HID_KEYBOARD);
    bleServer->getAdvertising()->addServiceUUID((uint16_t)0x1812);
    bleServer->getAdvertising()->start();

    Serial.println("SBR - XXX13");

    WiFi.persistent(false);

    if (!WifiEspNowBroadcast.begin("ESPNOW", 3))
    {
        Serial.println("WifiEspNowBroadcast.begin() failed.");

        ESP.restart();
    }

    WifiEspNowBroadcast.onReceive(
        [&](const uint8_t mac[6], const uint8_t *buf, size_t count, void *cbarg) {
            Serial.printf("Message from %02X:%02X:%02X:%02X:%02X:%02X\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

            for (int i = 0; i < count; ++i)
            {
                Serial.print(static_cast<char>(buf[i]));
            }

            Serial.println();
        },
        nullptr);

    Serial.println("ESPNOW");
}

void SbrXXX13::loop()
{
    //Serial.println("SbrXXX13::loop");

    sleepAWhileCount++;

    if (SbrXXX13::isConnected())
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

    WifiEspNowBroadcast.loop();
}

bool SbrXXX13::isConnected()
{
    return connected;
}

uint32_t SbrXXX13::sleepAWhileCount = 0;
bool SbrXXX13::connected = false;
int8_t SbrXXX13::x = 0;
int8_t SbrXXX13::y = 0;
