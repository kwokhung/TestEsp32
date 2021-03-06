#pragma once

#include <Arduino.h>
#include <HardwareSerial.h>
#include <boards/realboard.hpp>
#include <Wire.h>
#include <MPU6050.h>

#include "EtkVector.h"
#include "EtkQuaternion.h"

class Esp32Board : public hf::RealBoard
{
public:
  Esp32Board();

  void init(void) override;
  bool getQuaternion(float quat[4]) override;
  bool getGyroRates(float gyroRates[3]) override;
  uint32_t getMicroseconds() override;
  void writeMotor(uint8_t index, float value) override;

  bool getAccelerometer(float accelGs[3]);
  void readMpu();

private:
  MPU6050 *mpu;
  int16_t mpuValues[7];
};
