#include <HardwareSerial.h>
#include <BLEDevice.h>

#include "Thermometer.h"
#include "Temperature.h"
#include "Ble.h"

Ble::Ble(std::string name, char *serviceUuid, char *characteristicUuid)
    : name(name),
      serviceUuid(serviceUuid),
      characteristicUuid(characteristicUuid)
{
    Thermometer::isConnected = false;
    Temperature::value = 0;
}

void Ble::setup()
{
    BLEDevice::init(name);

    pServer = BLEDevice::createServer();

    new Thermometer(pServer, serviceUuid, characteristicUuid);

    pServer->getAdvertising()->start();

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
