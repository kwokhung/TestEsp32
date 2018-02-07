#ifndef SbrPid_h
#define SbrPid_h

#include <string>
#include <string.h>
#include <FreeRTOS.h>
#include <Arduino.h>

#include "SbrMpu.h"
#include "SbrMotor.h"
#include "SbrControl.h"

#define PRINT_PERIOD 100000 // print period in micros

#define MAX_PID_OUTPUT 500
#define MIN_CONTROL_ERR 1

class SbrPid
{
public:
  SbrPid(std::string name, SbrMpu *sbrMpu, SbrMotor *sbrMotor);

  void setup();

  static void startUp(void *parameter);

  TaskHandle_t task;

  static SbrMpu *sbrMpu;
  static SbrMotor *sbrMotor;
  static float BASE_Kp, BASE_Ki, BASE_Kd;
  static float Kp, Ki, Kd;
  static float angleSetpoint, selfBalanceAngleSetpoint;
  static float pidOutput, pidError, pidLastError, integralErr, positionErr, serialControlErr, prevSerialControlErr, errorDerivative;
  static float MAX_CONTROL_OR_POSITION_ERR;
  static float MAX_CONTROL_ERR_INCREMENT;
  static float roll, pitch, rollAcc, pitchAcc;
  static uint32_t loop_timer;
  static uint32_t print_timer;

private:
  std::string name;
};

#endif