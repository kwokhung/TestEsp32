#pragma once

#include <Arduino.h>
#include <HardwareSerial.h>
#include <BLEDevice.h>
#include <BLEHidDevice.h>
#include <HIDKeyboardTypes.h>

#include "SbrBase.hpp"
#include "Keyboard.hpp"
#include "Hid.hpp"

#define DATA_LENGTH 512  /*!<Data buffer length for test buffer*/
#define RW_TEST_LENGTH 4 /*!<Data length for r/w test, any value from 0-DATA_LENGTH*/

class SbrXXX11 : public SbrBase<SbrXXX11>, public BLEServerCallbacks
{
public:
  friend class SbrBase;

  void onConnect(BLEServer *bleServer);
  void onDisconnect(BLEServer *bleServer);
  static bool isConnected();

private:
  SbrXXX11(std::string name)
      : SbrBase(name)
  {
  }
  void setup() override;
  void loop() override;

  BLEServer *bleServer;
  static bool connected;
  static uint8_t *data;
};
