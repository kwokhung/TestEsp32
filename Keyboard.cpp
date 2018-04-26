#include "Keyboard.hpp"

Keyboard::Keyboard(BLEServer *bleServer, char *keyboardServiceUuid, char *hidCharacteristicUuid)
    : bleServer(bleServer)
{
    keyboardService = bleServer->createService(keyboardServiceUuid);

    Hid::init(keyboardService, hidCharacteristicUuid);

    keyboardService->start();
}

Keyboard *Keyboard::getSingleTon(BLEServer *bleServer, char *keyboardServiceUuid, char *hidCharacteristicUuid)
{
    if (singleTon == NULL)
    {
        singleTon = new Keyboard(bleServer, keyboardServiceUuid, hidCharacteristicUuid);
    }

    return (singleTon);
}

void Keyboard::init(BLEServer *bleServer, char *keyboardServiceUuid, char *hidCharacteristicUuid)
{
    getSingleTon(bleServer, keyboardServiceUuid, hidCharacteristicUuid);
}

Keyboard *Keyboard::singleTon = NULL;
BLEService *Keyboard::keyboardService;
