#include "SbrMpu.h"
#include "SbrMotor.h"
#include "SbrControl.h"
#include "SbrConfig.h"

SbrMpu *sbrMpu = new SbrMpu("Self balancing Robot - MPU");
SbrMotor *sbrMotor = new SbrMotor("Self balancing Robot - Motor");
SbrControl *sbrControl = new SbrControl("Self balancing Robot - Control");
SbrConfig *sbrConfig = new SbrConfig("Self balancing Robot - Config", sbrMotor);

#define PRINT_PERIOD 100000 // print period in micros

#define MAX_PID_OUTPUT 500
#define MIN_CONTROL_ERR 1

float BASE_Kp = 100.0, BASE_Ki = 5.0, BASE_Kd = 130.0;
float Kp = BASE_Kp, Ki = BASE_Ki, Kd = BASE_Kd;
float angleSetpoint = 0, selfBalanceAngleSetpoint = 0;
float pidOutput, pidError, pidLastError, integralErr, positionErr, serialControlErr, prevSerialControlErr, errorDerivative;

float MAX_CONTROL_OR_POSITION_ERR = MAX_PID_OUTPUT / Kp;
float MAX_CONTROL_ERR_INCREMENT = MAX_CONTROL_OR_POSITION_ERR / 400;

uint32_t loop_timer;
uint32_t print_timer;

float roll, pitch, rollAcc, pitchAcc;

void setup()
{
  Serial.begin(500000);

  xTaskCreate(SbrMpu::startUp, "SBR - MPU", 10000, sbrMpu, 1, &sbrMpu->task);
  xTaskCreate(SbrMotor::startUp, "SBR - Motor", 10000, sbrMotor, 1, &sbrMotor->task);
  xTaskCreate(SbrControl::startUp, "SBR - Control", 10000, sbrControl, 2, &sbrControl->task);
  xTaskCreate(SbrConfig::startUp, "SBR - Config", 10000, sbrConfig, 1, &sbrConfig->task);

  loop_timer = micros() + PERIOD;
  print_timer = micros() + PRINT_PERIOD;
}

void loop()
{
  sbrMpu->getAcceleration(&sbrMpu->accX, &sbrMpu->accY, &sbrMpu->accZ);
  rollAcc = asin((float)sbrMpu->accX / ACC_SCALE_FACTOR) * RAD_TO_DEG;
  pitchAcc = asin((float)sbrMpu->accY / ACC_SCALE_FACTOR) * RAD_TO_DEG;

  sbrMpu->getRotation(&sbrMpu->gyroX, &sbrMpu->gyroY, &sbrMpu->gyroZ);
  // roll vs pitch depends on how the MPU is installed in the robot
  roll -= sbrMpu->gyroY * SbrMpu::GYRO_RAW_TO_DEGS;
  pitch += sbrMpu->gyroX * SbrMpu::GYRO_RAW_TO_DEGS;
  // sin() has to be applied on radians
  //  roll += pitch * sin((float)gyroZ * SbrMpu::GYRO_RAW_TO_DEGS * DEG_TO_RAD);
  //  pitch -= roll * sin((float)gyroZ * SbrMpu::GYRO_RAW_TO_DEGS * DEG_TO_RAD);

  roll = roll * 0.999 + rollAcc * 0.001;
  pitch = pitch * 0.999 + pitchAcc * 0.001;

  // apply PID algo

  // The selfBalanceAngleSetpoint variable is automatically changed to make sure that the robot stays balanced all the time.
  positionErr = SbrMpu::constrf(sbrMotor->currentPos / (float)1000, -MAX_CONTROL_OR_POSITION_ERR, MAX_CONTROL_OR_POSITION_ERR);
  serialControlErr = 0;

  if (SbrControl::isValidJoystickValue(SbrControl::joystickY))
  {
    serialControlErr = SbrMpu::constrf((SbrControl::joystickY - 130) / (float)15, -MAX_CONTROL_OR_POSITION_ERR, MAX_CONTROL_OR_POSITION_ERR);

    // this control has to change slowly/gradually to avoid shaking the robot
    if (serialControlErr < prevSerialControlErr)
    {
      serialControlErr = _max(serialControlErr, prevSerialControlErr - MAX_CONTROL_ERR_INCREMENT);
    }
    else
    {
      serialControlErr = _min(serialControlErr, prevSerialControlErr + MAX_CONTROL_ERR_INCREMENT);
    }

    prevSerialControlErr = serialControlErr;
  }

  pidError = pitch - angleSetpoint - selfBalanceAngleSetpoint;

  // either no manual / serial control -> try to keep the position or follow manual control
  if (abs(serialControlErr) > MIN_CONTROL_ERR)
  {
    pidError += serialControlErr > 0 ? serialControlErr - MIN_CONTROL_ERR : serialControlErr + MIN_CONTROL_ERR;
    // re-init position so it doesn't try to go back when getting out of manual control mode
    sbrMotor->currentPos = 0;
  }
  else
  {
    pidError += positionErr;
  }

  integralErr = SbrMpu::constrf(integralErr + Ki * pidError, -MAX_PID_OUTPUT, MAX_PID_OUTPUT);
  errorDerivative = pidError - pidLastError;

  pidOutput = Kp * pidError + integralErr + Kd * errorDerivative;

  if (pidOutput < 5 && pidOutput > -5)
  {
    pidOutput = 0; //Create a dead-band to stop the motors when the robot is balanced
  }

  if (pitch > 30 || pitch < -30)
  {
    //If the robot tips over
    pidOutput = 0;
    integralErr = 0;
    selfBalanceAngleSetpoint = 0;
  }

  // store error for next loop
  pidLastError = pidError;

  int16_t rotation = 0;

  if (SbrControl::isValidJoystickValue(SbrControl::joystickX))
  {
    rotation = SbrMpu::constrf((float)(SbrControl::joystickX - 130), -MAX_PID_OUTPUT, MAX_PID_OUTPUT) * (MAX_SPEED / MAX_PID_OUTPUT);
  }

  if (micros() >= print_timer)
  {
    Serial.print(positionErr);
    Serial.print("  -  ");
    Serial.print(rotation);
    Serial.print(" / ");
    Serial.println(serialControlErr);
    //Serial.print(pitch); Serial.print(" / ");Serial.print(errorDerivative);Serial.print(" - ");Serial.println(selfBalanceAngleSetpoint);
    //Serial.print(accX); Serial.print(" / ");Serial.print(accY); Serial.print(" / ");Serial.print(accZ); Serial.print(" / ");Serial.print(gyroX); Serial.print(" / ");Serial.print(gyroY);Serial.print(" / ");Serial.println(gyroZ);
    print_timer += PRINT_PERIOD;
  }

  //The self balancing point is adjusted when there is not forward or backwards movement from the transmitter. This way the robot will always find it's balancing point
  //  if(angleSetpoint == 0) {                                  //If the setpoint is zero degrees
  //    if(pidOutput < 0) selfBalanceAngleSetpoint -= 0.0015;   //Increase the self_balance_pid_setpoint if the robot is still moving forward
  //    if(pidOutput > 0) selfBalanceAngleSetpoint += 0.0015;   //Decrease the self_balance_pid_setpoint if the robot is still moving backward
  //  }

  sbrMotor->setSpeed(SbrMpu::constrf(pidOutput, -MAX_PID_OUTPUT, MAX_PID_OUTPUT) * (MAX_SPEED / MAX_PID_OUTPUT), rotation);

  // The angle calculations are tuned for a loop time of PERIOD milliseconds.
  // To make sure every loop is exactly that, a wait loop is created by setting the loop_timer
  if (loop_timer <= micros())
  {
    Serial.println("ERROR loop too short !");
  }

  while (loop_timer > micros())
  {
  }

  loop_timer += PERIOD;
}
