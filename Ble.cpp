#include <HardwareSerial.h>
#include <BLEDevice.h>

#include "Thermometer.h"
#include "Temperature.h"
#include "Ble.h"

Ble::Ble(std::string name, char *thermometerServiceUuid, char *temperatureCharacteristicUuid)
    : name(name),
      thermometerServiceUuid(thermometerServiceUuid),
      temperatureCharacteristicUuid(temperatureCharacteristicUuid)
{
    Thermometer::isConnected = false;
}

void Ble::setup()
{
    BLEDevice::init(name);

    bleServer = BLEDevice::createServer();

    new Thermometer(bleServer, thermometerServiceUuid, temperatureCharacteristicUuid);

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
