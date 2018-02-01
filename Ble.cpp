#include <HardwareSerial.h>
#include <BLEDevice.h>

#include "Thermometer.h"
#include "Temperature.h"
#include "Ble.h"

Ble::Ble(std::string name, char *thermometerUuid, char *temperatureUuid)
    : name(name),
      thermometerUuid(thermometerUuid),
      temperatureUuid(temperatureUuid)
{
    Thermometer::isConnected = false;
    Temperature::value = 0;
}

void Ble::setup()
{
    BLEDevice::init(name);

    bleServer = BLEDevice::createServer();

    new Thermometer(bleServer, thermometerUuid, temperatureUuid);

    bleServer->getAdvertising()->start();

    Serial.println("Waiting a client connection...");
}

void Ble::notify()
{
    if (Thermometer::isConnected)
    {
        Serial.printf("*** Sent Value: %d ***\n", Temperature::getValue());

        Temperature::notify();

        Temperature::setValue(Temperature::getValue() + 1);
    }

    delay(1000);
}
