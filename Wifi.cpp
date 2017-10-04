#include <Arduino.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "OLed.h"
#include "Wifi.h"

Wifi::Wifi(OLed &oLed)
    : server(new ESP8266WebServer(80)),
      oLed(&oLed)
{
}

void Wifi::setup()
{
    String ssid = "";

    for (int i = 0; i < 32; i++)
    {
        ssid += char(EEPROM.read(i));
    }
    //ssid = "xxx";

    String password = "";

    for (int i = 32; i < 96; i++)
    {
        password += char(EEPROM.read(i));
    }
    //ssid = "yyy";

    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(const_cast<char *>(ssid.c_str()), const_cast<char *>(password.c_str()));

    int percentage = 0;

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        oLed->connecting(ssid, password, percentage);

        percentage += 5;

        if (percentage > 100)
        {
            break;
        }
    }

    if (percentage > 100)
    {
        randomSeed(analogRead(0));
        long randNumber = random(100, 1000);
        String mySsid = "ESP" + String(random(100, 1000), DEC);
        //String myPassword = String(random(10000000, 100000000), DEC);
        String myPassword = "12345678";

        Serial.println("");
        Serial.println("Configuring access point...");
        Serial.println(mySsid);
        Serial.println(myPassword);

        WiFi.softAP(const_cast<char *>(mySsid.c_str()), const_cast<char *>(myPassword.c_str()));

        IPAddress myIP = WiFi.softAPIP();

        connectInfo = mySsid + "/" + myPassword + "/" + myIP.toString();

        Serial.print("Connect Info: ");
        Serial.println(connectInfo);

        oLed->beAccessPoint(mySsid, myPassword, &myIP);

        server->on("/", HTTP_GET, [&]() {
            server->send(200, "text/html", "<h1>You are connected</h1><p>" + connectInfo + "</p>");
        });

        server->on("/", HTTP_POST, [&]() {
            DynamicJsonBuffer jsonBuffer;
            JsonObject &payloadJson = jsonBuffer.parseObject(server->arg("plain"));

            //String payloadString;
            //payloadJson.printTo(payloadString);
            //server->send(200, "text/json", payloadString);

            String newSsid = payloadJson["ssid"].asString();
            String newPassword = payloadJson["password"].asString();

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

            server->send(200, "application/json", "{success:true}");
            delay(1000);
            //server->stop();

            ESP.reset();
        });

        server->begin();

        while (true)
        {
            server->handleClient();
        }
    }
    else
    {
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());

        oLed->connected(ssid, WiFi.localIP().toString());
    }
}
