#ifndef SbrMotor_h
#define SbrMotor_h

#include <string>
#include <string.h>
#include <FreeRTOS.h>
#include <Arduino.h>

#include "SbrBase.h"

#define MOT_L_CHANNEL 2
#define MOT_R_CHANNEL 1

#define MOT_L_STP 14
#define MOT_R_STP 33

#define MOT_L_ENB 26
#define MOT_R_ENB 32

#define MOT_L_DIR 27
#define MOT_R_DIR 25

#define MAX_SPEED 20000

class SbrMotor : public SbrBase<SbrMotor>
{
public:
  friend class SbrBase;
  void setSpeed(int16_t s, int16_t rotation);

  uint32_t prevSpeedStart;
  int16_t prevSpeed;
  int32_t currentPos;

private:
  SbrMotor(std::string name)
      : SbrBase(name)
  {
  }
  void setup() override;
  void loop() override;
  void disableL(bool orEnable);
  void disableR(bool orEnable);
  void forwardL(bool orBack);
  void forwardR(bool orBack);
};

#endif