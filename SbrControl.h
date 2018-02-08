#ifndef SbrControl_h
#define SbrControl_h

#include <string>
#include <string.h>
#include <FreeRTOS.h>
#include <Arduino.h>
#include <HardwareSerial.h>

#define MAX_SPEED 20000

class SbrControl
{
public:
  SbrControl(std::string name);

  void setup();
  void loop();

  static void startUp(void *parameter);
  static boolean startNewMsg(uint8_t c);
  static boolean isValidJoystickValue(uint8_t joystick);

  TaskHandle_t task;

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
  std::string name;
};

#endif