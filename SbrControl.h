#ifndef SbrControl_h
#define SbrControl_h

#include <string>
#include <string.h>
#include <FreeRTOS.h>
#include <Arduino.h>
#include <HardwareSerial.h>

#include "SbrBase.h"

#define MAX_SPEED 20000

class SbrControl : public SbrBase<SbrControl>
{
public:
  SbrControl(std::string name)
      : SbrBase(name)
  {
  }

  void setup() override;
  void loop() override;

  //static SbrControl *getSingleTon(std::string name);
  //static void startUp(void *parameter);
  static boolean startNewMsg(uint8_t c);
  static boolean isValidJoystickValue(uint8_t joystick);

  //std::string name;
  //TaskHandle_t task;

  static HardwareSerial &SerialControl;
  static uint8_t currChar;
  static uint8_t _prevChar;
  static boolean _readingMsg;
  static uint8_t _msgPos;
  static boolean _validData;
  static uint8_t _msg[6];
  static uint8_t joystickX;
  static uint8_t joystickY;

private:
  //SbrControl(std::string name);

  //static SbrControl *singleTon;
};

#endif