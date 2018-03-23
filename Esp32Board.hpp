#pragma once

#include <Arduino.h>
#include <HardwareSerial.h>
#include <boards/real/realboard.hpp>
#include <Wire.h>
#include <MPU6050.h>

class Esp32Board : public hf::RealBoard
{
public:
  Esp32Board();

  void init(void) override;
  bool getEulerAngles(float eulerAngles[3]) override;
  bool getGyroRates(float gyroRates[3]) override;
  uint32_t getMicroseconds() override;
  void writeMotor(uint8_t index, float value) override;

  bool getAccelerometer(float accelGs[3]);
  void readMpu();

private:
  MPU6050 *mpu;
  int16_t mpuValues[7];
};
