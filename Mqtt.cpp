#include <EEPROM.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "Led.h"
#include "OLed.h"
#include "Mqtt.h"

Mqtt::Mqtt(Gprs &gprs, char *mqttUrl, Led &led, OLed &oLed)
    : gprs(&gprs),
      //client(new PubSubClient(*new WiFiClient())),
      client(new PubSubClient(*this->gprs->getGsmClient())),
      mqttUrl(mqttUrl),
      led(&led),
      oLed(&oLed)
{
    //WiFiClient* espClient = new WiFiClient();
    //WiFiClientSecure* espClient = new WiFiClientSecure;
    //client = new PubSubClient(*new WiFiClient());
    //this->gprs = &gprs;
    //client = new PubSubClient(*this->gprs->getGsmClient());
}

void Mqtt::setup()
{
    client->setServer(mqttUrl, 1883);
    //client->setServer(mqttUrl, 1884);

    client->setCallback([&](char *topic, byte *payload, unsigned int length) {
        Serial.print("Message arrived [");
        Serial.print(topic);
        Serial.print("] ");

        String payloadString = "";

        for (int i = 0; i < length; i++)
        {
            payloadString += (char)payload[i];
        }

        DynamicJsonBuffer jsonBuffer;
        JsonObject &payloadJson = jsonBuffer.parseObject(payloadString);

        Serial.print('*');
        Serial.print(payloadString);

        String toDo = payloadJson["what"]["toDo"].asString();

        if (toDo == "reset")
        {
            String newSsid = payloadJson["what"]["details"]["ssid"].asString();
            String newPassword = payloadJson["what"]["details"]["password"].asString();

            oLed->reset(newSsid, newPassword);

            for (int i = 0; i < 96; i++)
            {
                EEPROM.write(i, 0);
            }

            for (int i = 0; i < newSsid.length(); i++)
            {
                EEPROM.write(i, newSsid[i]);
            }

            for (int i = 0; i < newPassword.length(); i++)
            {
                EEPROM.write(32 + i, newPassword[i]);
            }

            EEPROM.commit();

            ESP.reset();
        }
        else
        {
            led->lightR(payloadJson["RVALUE"].as<int>());
            led->lightG(payloadJson["GVALUE"].as<int>());
            led->lightB(payloadJson["BVALUE"].as<int>());
        }

        Serial.println('*');
    });

    reconnect();
}

void Mqtt::loop()
{
    if (!client->connected())
    {
        reconnect();
    }

    client->loop();
}

void Mqtt::reconnect()
{
    while (!client->connected())
    {
        Serial.print("Attempting MQTT connection...");

        if (client->connect("ad7cad07680c47ff80677b3c19bbe6dc", "mbltest01/nodemcu01", "e61m/mza6z5HY0eD4n/sbagP6mkDZeFfmmxSh5KER0w="))
        {
            Serial.println("connected");

            client->publish("letv1s01", "{\"RVALUE\":0,\"GVALUE\":0,\"BVALUE\":0}");
            client->subscribe("nodemcu01");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client->state());
            Serial.println(" try again in 5 seconds");

            delay(5000);
        }
    }
}
