#ifndef SbrXXX04_h
#define SbrXXX04_h

#include <Kalman.h>
#include <MPU6050.h>

#include "SbrBase.h"

#define Rad2Deg 57.295779513f //将弧度转为角度的乘数

class SbrXXX04 : public SbrBase<SbrXXX04>
{
public:
  friend class SbrBase;
  void readMpu();
  void calibrate();
  void rectify();
  float getRoll();
  float getPitch();

private:
  SbrXXX04(std::string name)
      : SbrBase(name)
  {
  }
  void setup() override;
  void loop() override;

  MPU6050 *mpu;
  int16_t mpuValues[7];
  int16_t offsetValues[7]; //校准数据
  float calculatedValues[7];
  unsigned long prevTime; //上一次读数的时间
  float prevRoll;         //上一次滤波得到的Roll角
  float prevPitch;        //上一次滤波得到的Pitch角
  Kalman kalmanRoll;      //Roll角滤波器
  Kalman kalmanPitch;     //Pitch角滤波器
};

#endif