#include <HardwareSerial.h>

#include "Temperature.h"

void Temperature::onWrite(BLECharacteristic *pCharacteristic)
{
    value = *pCharacteristic->getValue().data();
    Serial.println("*********");
    Serial.printf("New value: %d\n", value);
    Serial.println("*********");
}

uint8_t Temperature::value;
