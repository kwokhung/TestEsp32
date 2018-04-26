#include "Keyboard.hpp"

Keyboard::Keyboard(BLEServer *bleServer)
    : bleServer(bleServer)
{
    keyboardService = bleServer->createService((uint16_t)0x180a);
    keyboardService1 = bleServer->createService((uint16_t)0x1812, 30);

    Hid::init(keyboardService, keyboardService1);
    //setupCharacteristics();

    keyboardService->start();
    keyboardService1->start();
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
BLEService *Keyboard::keyboardService;
BLEService *Keyboard::keyboardService1;
