#ifndef SbrMotor_h
#define SbrMotor_h

#include <string>
#include <string.h>
#include <FreeRTOS.h>
#include <Arduino.h>

#define MOT_R_ENB 32
#define MOT_R_STP 33
#define MOT_R_DIR 25
#define MOT_R_CHANNEL 1 // for the ledc library

#define MOT_L_ENB 26
#define MOT_L_STP 14
#define MOT_L_DIR 27
#define MOT_L_CHANNEL 2 // for the ledc library

#define MAX_SPEED 20000

class SbrMotor
{
public:
  SbrMotor(std::string name);

  void setup();
  void setup_motor();
  void disableL(bool orEnable);
  void disableR(bool orEnable);
  void forwardL(bool orBack);
  void forwardR(bool orBack);
  void setSpeed(int16_t s, int16_t rotation);

  static void startUp(void *parameter);

  TaskHandle_t task;
  uint32_t prevSpeedStart;
  int16_t prevSpeed;
  int32_t currentPos;

private:
  std::string name;
};

#endif