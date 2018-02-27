#ifndef SbrXXX01_h
#define SbrXXX01_h

#include <driver/gpio.h>

#include "SbrBase.h"

#define SDAPIN (GPIO_NUM_21)
#define SCLPIN (GPIO_NUM_22)

#define MPU6050_ADDR 0x68       // MPU-6050 device address
#define MPU6050_SMPLRT_DIV 0x19 // MPU-6050 register address
#define MPU6050_CONFIG 0x1a
#define MPU6050_GYRO_CONFIG 0x1b
#define MPU6050_ACCEL_CONFIG 0x1c
#define MPU6050_WHO_AM_I 0x75
#define MPU6050_PWR_MGMT_1 0x6b

class SbrXXX01 : public SbrBase<SbrXXX01>
{
public:
  friend class SbrBase;
  uint8_t readMPU6050(uint8_t reg);
  void writeMPU6050(uint8_t reg, uint8_t data);
  void calcRotation();

private:
  SbrXXX01(std::string name)
      : SbrBase(name)
  {
  }
  void setup() override;
  void loop() override;

  float dpsX, dpsY, dpsZ;
  double offsetX, offsetY, offsetZ;
  float acc_x, acc_y, acc_z, acc_angle_x, acc_angle_y;
  float interval, preInterval;
  double gyro_angle_x, gyro_angle_y, gyro_angle_z;
  float angleX, angleY, angleZ;

  int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
};

#endif