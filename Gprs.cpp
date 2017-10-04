//#define SerialAT Serial
#include <SoftwareSerial.h>
SoftwareSerial SerialAT(13, 15); // RX, TX
#define TINY_GSM_MODEM_A6

#include "Gprs.h"

Gprs::Gprs(char *apn)
    : modem(new TinyGsm(SerialAT)),
      gsmClient(new TinyGsmClient(*modem))
{
}

void Gprs::setup()
{
    const char apn[] = "mobile.lte.three.com.hk";
    const char user[] = "";
    const char pass[] = "";

    // Set GSM module baud rate
    SerialAT.begin(115200);
    delay(3000);

    // Restart takes quite some time
    // To skip it, call init() instead of restart()
    Serial.println("Initializing modem...");
    modem->restart();

    String modemInfo = modem->getModemInfo();
    Serial.print("Modem: ");
    Serial.println(modemInfo);

    // Unlock your SIM card with a PIN
    //modem->simUnlock("1234");

    Serial.print("Waiting for network...");

    if (!modem->waitForNetwork())
    {
        Serial.println(" fail");

        while (true)
        {
        }
    }

    Serial.println(" OK");

    Serial.print("Connecting to ");
    Serial.print(apn);

    if (!modem->gprsConnect(apn, user, pass))
    {
        Serial.println(" fail");

        while (true)
        {
        }
    }

    Serial.println(" OK");
}

TinyGsmClient *Gprs::getGsmClient()
{
    return gsmClient;
}
