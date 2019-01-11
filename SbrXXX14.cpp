#include "SbrXXX14.hpp"

void SbrXXX14::setup()
{
    Serial.println("SbrXXX14::setup");

    queueSize = 10;
    queue = xQueueCreate(queueSize, sizeof(int));

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

void SbrXXX14::loop()
{
    //Serial.println("SbrXXX14::loop");

    WifiEspNowBroadcast.loop();

    sleepAWhile(1000);
}
