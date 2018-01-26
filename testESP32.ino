#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

BLEServer *pServer;
BLEService *pService;
BLECharacteristic *pCharacteristic;

bool deviceConnected = false;
uint8_t txValue = 0;

#define SERVICE_UUID "ffe0" // UART service UUID
#define CHARACTERISTIC_UUID_TX "ffe1"

class MyServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        deviceConnected = true;
    };

    void onDisconnect(BLEServer *pServer)
    {
        deviceConnected = false;
    }
};

void setup()
{
    Serial.begin(115200);

    BLEDevice::init("Notify Service");

    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());
    pService = pServer->createService(SERVICE_UUID);
    pCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY);

    pService->start();
    pServer->getAdvertising()->start();

    Serial.println("Waiting a client connection to notify...");
}

void loop()
{

    if (deviceConnected)
    {
        Serial.printf("*** Sent Value: %d ***\n", txValue);
        pCharacteristic->setValue(&txValue, 1);
        pCharacteristic->notify();
        txValue++;
    }

    delay(1000);
}
