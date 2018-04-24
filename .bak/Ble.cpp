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
    task = NULL;
}

void Ble::setup()
{
    BLEDevice::init(name);

    bleServer = BLEDevice::createServer();

    Thermometer::init(bleServer, thermometerServiceUuid, temperatureCharacteristicUuid);

    bleServer->getAdvertising()->start();

    Serial.println("Waiting a client connection...");
}

void Ble::notify()
{
    if (Thermometer::isConnected())
    {
        Serial.printf("*** Sent Value: %d ***\n", Temperature::getValue());

        Temperature::notify();

        Temperature::setValue(Temperature::getValue() + 1);
    }

    //delay(1000);
    TickType_t xLastWakeTime = xTaskGetTickCount();
    vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS( 1000 ) );
}

void Ble::startUp(void *parameter)
{
    Ble *ble = (Ble *)parameter;

    ble->setup();

    while (true)
    {
        ble->notify();
    }

    vTaskDelete(NULL);
}
