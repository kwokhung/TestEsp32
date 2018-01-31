#include <HardwareSerial.h>
#include <BLEDevice.h>
#include <BLE2902.h>

#include "Ble.h"

class Thermometer : public BLEServerCallbacks
{
  public:
    void onConnect(BLEServer *pServer);
    void onDisconnect(BLEServer *pServer);
    static bool isConnected;
};

void Thermometer::onDisconnect(BLEServer *pServer)
{
    isConnected = false;
}

void Thermometer::onConnect(BLEServer *pServer)
{
    isConnected = true;
}

bool Thermometer::isConnected;

class Temperature : public BLECharacteristicCallbacks
{
  public:
    void onWrite(BLECharacteristic *pCharacteristic);
    static uint8_t value;
};

void Temperature::onWrite(BLECharacteristic *pCharacteristic)
{
    value = *pCharacteristic->getValue().data();
    Serial.println("*********");
    Serial.printf("New value: %d\n", value);
    Serial.println("*********");
}

uint8_t Temperature::value;

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
    Serial.begin(115200);

    BLEDevice::init(name);

    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new Thermometer());
    pService = pServer->createService(serviceUuid);
    pCharacteristic = pService->createCharacteristic(
        characteristicUuid,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_NOTIFY);
    pCharacteristic->addDescriptor(new BLE2902());
    pCharacteristic->setCallbacks(new Temperature());

    pService->start();
    pServer->getAdvertising()->start();

    Serial.println("Waiting a client connection...");
}

void Ble::notify()
{
    if (Thermometer::isConnected)
    {
        Serial.printf("*** Sent Value: %d ***\n", Temperature::value);

        pCharacteristic->setValue(&Temperature::value, 1);
        pCharacteristic->notify();
        Temperature::value++;
    }

    delay(1000);
}
