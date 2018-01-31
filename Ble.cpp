#include <HardwareSerial.h>
#include <BLEDevice.h>
#include <BLE2902.h>

#include "Ble.h"

class MyServerCallbacks : public BLEServerCallbacks
{
  public:
    void onConnect(BLEServer *pServer)
    {
        deviceConnected = true;
    };

    void onDisconnect(BLEServer *pServer)
    {
        deviceConnected = false;
    }

    static bool deviceConnected;
};

bool MyServerCallbacks::deviceConnected;

class MyCallbacks : public BLECharacteristicCallbacks
{
  public:
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        //txValue = *((uint8_t *)pCharacteristic->getValue().data());
        txValue = *pCharacteristic->getValue().data();
        Serial.println("*********");
        Serial.printf("New value: %d\n", txValue);
        Serial.println("*********");
    }

    static uint8_t txValue;
};

uint8_t MyCallbacks::txValue;

Ble::Ble(std::string name)
    : name(name)
{
    MyServerCallbacks::deviceConnected = false;
    MyCallbacks::txValue = 0;
}

void Ble::setup()
{
    Serial.begin(115200);

    BLEDevice::init(name);

    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());
    pService = pServer->createService(SERVICE_UUID);
    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_NOTIFY);
    pCharacteristic->addDescriptor(new BLE2902());
    pCharacteristic->setCallbacks(new MyCallbacks());

    pService->start();
    pServer->getAdvertising()->start();

    Serial.println("Waiting a client connection...");
}

void Ble::notify()
{
    if (MyServerCallbacks::deviceConnected)
    {
        Serial.printf("*** Sent Value: %d ***\n", MyCallbacks::txValue);

        pCharacteristic->setValue(&MyCallbacks::txValue, 1);
        pCharacteristic->notify();
        MyCallbacks::txValue++;
    }

    delay(1000);
}
