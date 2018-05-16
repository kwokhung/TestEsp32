#pragma once

#include <Arduino.h>
#include <HardwareSerial.h>
#include <BLEDevice.h>
#include <BLEHidDevice.h>
#include <HIDKeyboardTypes.h>

#include "SbrBase.hpp"
#include "Keyboard.hpp"
#include "Hid.hpp"

#define DATA_LENGTH 512 /*!<Data buffer length for test buffer*/
#define RW_TEST_LENGTH 10 /*!<Data length for r/w test, any value from 0-DATA_LENGTH*/

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

  double Input;
  BLEServer *bleServer;
  static uint32_t sleepAWhileCount;
  static bool connected;
  static int8_t x;
  static int8_t y;
};
