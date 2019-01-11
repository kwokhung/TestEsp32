#ifndef SbrXXX02_h
#define SbrXXX02_h

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

class SbrXXX02 : public SbrBase<SbrXXX02>
{
public:
  friend class SbrBase;
  uint8_t readMPU6050(uint8_t reg);
  void writeMPU6050(uint8_t reg, uint8_t data);
  void readRawData();
  void calcRotation();

private:
  SbrXXX02(std::string name)
      : SbrBase(name)
  {
  }
  void setup() override;
  void loop() override;

  long acc_x, acc_y, acc_z;
  int temperature;
  int gyro_x, gyro_y, gyro_z;
  long gyro_x_cal, gyro_y_cal, gyro_z_cal;
  long loop_timer;
  float angle_pitch, angle_roll;
  long acc_total_vector;
  float angle_roll_acc, angle_pitch_acc;
  boolean set_gyro_angles;
  float angle_pitch_output, angle_roll_output;
};

#endif