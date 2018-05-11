#include "Keyboard.hpp"

Keyboard::Keyboard(BLEServer *bleServer)
    : bleServer(bleServer)
{
    deviceInformationService = bleServer->createService((uint16_t)0x180a);
    humanInterfaceDeviceService = bleServer->createService((uint16_t)0x1812, 40);
    batteryService = bleServer->createService((uint16_t)0x180f);

    Hid::init(deviceInformationService, humanInterfaceDeviceService, batteryService);

    deviceInformationService->start();
    humanInterfaceDeviceService->start();
    batteryService->start();
}

Keyboard *Keyboard::getSingleTon(BLEServer *bleServer)
{
    if (singleTon == NULL)
    {
        singleTon = new Keyboard(bleServer);
    }

    return (singleTon);
}

void Keyboard::init(BLEServer *bleServer)
{
    getSingleTon(bleServer);
}

Keyboard *Keyboard::singleTon = NULL;
BLEService *Keyboard::deviceInformationService;
BLEService *Keyboard::humanInterfaceDeviceService;
BLEService *Keyboard::batteryService;

