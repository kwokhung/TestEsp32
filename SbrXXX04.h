#ifndef SbrXXX04_h
#define SbrXXX04_h

#include <Kalman.h>
#include <MPU6050.h>

#include "SbrBase.h"

#define fRad2Deg 57.295779513f //将弧度转为角度的乘数
#define MPU 0x68               //MPU-6050的I2C地址
#define nValCnt 7              //一次读取寄存器的数量
#define nCalibTimes 1000       //校准时读数的次数

class SbrXXX04 : public SbrBase<SbrXXX04>
{
public:
  friend class SbrBase;
  void ReadAccGyr(int16_t *pVals);
  void Calibration();
  float GetRoll(float *pRealVals, float fNorm);
  float GetPitch(float *pRealVals, float fNorm);
  void Rectify(int16_t *pReadout, float *pRealVals);

  int16_t calibData[nValCnt]; //校准数据

  unsigned long nLastTime; //上一次读数的时间
  float fLastRoll;         //上一次滤波得到的Roll角
  float fLastPitch;        //上一次滤波得到的Pitch角
  Kalman kalmanRoll;       //Roll角滤波器
  Kalman kalmanPitch;      //Pitch角滤波器

private:
  SbrXXX04(std::string name)
      : SbrBase(name)
  {
  }
  void setup() override;
  void loop() override;

  MPU6050 *accelgyro;
};

#endif