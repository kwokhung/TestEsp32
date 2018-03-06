#include <HardwareSerial.h>
#include <Wire.h>
#include <Math.h>

#include "SbrXXX04.h"

void SbrXXX04::setup()
{
    Serial.println("SbrXXX04::setup");

    queueSize = 10;
    queue = xQueueCreate(queueSize, sizeof(int));

    nLastTime = 0;     //上一次读数的时间
    fLastRoll = 0.0f;  //上一次滤波得到的Roll角
    fLastPitch = 0.0f; //上一次滤波得到的Pitch角

    Wire.begin(); //初始化Wire库

    accelgyro = new MPU6050();

    Serial.println("Initializing I2C devices...");
    accelgyro->initialize();

    Serial.println("Testing device connections...");
    Serial.println(accelgyro->testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    Calibration();        //执行校准
    nLastTime = micros(); //记录当前时间
}

void SbrXXX04::loop()
{
    Serial.println("SbrXXX04::loop");

    int16_t readouts[nValCnt];
    ReadAccGyr(readouts); //读出测量值

    float realVals[7];
    Rectify(readouts, realVals); //根据校准的偏移量进行纠正

    //计算加速度向量的模长，均以g为单位
    float fNorm = sqrt(realVals[0] * realVals[0] + realVals[1] * realVals[1] + realVals[2] * realVals[2]);
    float fRoll = GetRoll(realVals, fNorm); //计算Roll角
    if (realVals[1] > 0)
    {
        fRoll = -fRoll;
    }
    float fPitch = GetPitch(realVals, fNorm); //计算Pitch角
    if (realVals[0] < 0)
    {
        fPitch = -fPitch;
    }

    //计算两次测量的时间间隔dt，以秒为单位
    unsigned long nCurTime = micros();
    float dt = (double)(nCurTime - nLastTime) / 1000000.0;
    //对Roll角和Pitch角进行卡尔曼滤波
    float fNewRoll = kalmanRoll.getAngle(fRoll, realVals[4], dt);
    float fNewPitch = kalmanPitch.getAngle(fPitch, realVals[5], dt);
    //跟据滤波值计算角度速
    float fRollRate = (fNewRoll - fLastRoll) / dt;
    float fPitchRate = (fNewPitch - fLastPitch) / dt;

    //更新Roll角和Pitch角
    fLastRoll = fNewRoll;
    fLastPitch = fNewPitch;
    //更新本次测的时间
    nLastTime = nCurTime;

    //向串口打印输出Roll角和Pitch角，运行时在Arduino的串口监视器中查看
    Serial.print("Roll:");
    Serial.print(fNewRoll);
    Serial.print('(');
    Serial.print(fRollRate);
    Serial.print("),\tPitch:");
    Serial.print(fNewPitch);
    Serial.print('(');
    Serial.print(fPitchRate);
    Serial.print(")\n");

    //sleepAWhile(1000);
    esp_task_wdt_reset();
    delayMicroseconds(10);
}

//从MPU6050读出加速度计三个分量、温度和三个角速度计
//保存在指定的数组中
void SbrXXX04::ReadAccGyr(int16_t *pVals)
{
    accelgyro->getMotion6(&pVals[0], &pVals[1], &pVals[2], &pVals[4], &pVals[5], &pVals[6]);
    pVals[3] = accelgyro->getTemperature();

    Serial.print("a/g:\t");
    Serial.print(pVals[0] * 2.0 / 32768);
    Serial.print("\t");
    Serial.print(pVals[1] * 2.0 / 32768);
    Serial.print("\t");
    Serial.print(pVals[2] * 2.0 / 32768);
    Serial.print("\t");
    Serial.print(pVals[3] / 340.00 + 36.53);
    Serial.print("\t");
    Serial.print(pVals[4] * 250.0 / 32768);
    Serial.print("\t");
    Serial.print(pVals[5] * 250.0 / 32768);
    Serial.print("\t");
    Serial.println(pVals[6] * 250.0 / 32768);
}

//对大量读数进行统计，校准平均偏移量
void SbrXXX04::Calibration()
{
    float valSums[7] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    //先求和
    for (int i = 0; i < nCalibTimes; ++i)
    {
        int16_t mpuVals[nValCnt];
        ReadAccGyr(mpuVals);
        for (int j = 0; j < nValCnt; ++j)
        {
            valSums[j] += mpuVals[j];
        }
    }
    //再求平均
    for (int i = 0; i < nValCnt; ++i)
    {
        calibData[i] = int16_t(valSums[i] / nCalibTimes);
    }
    //calibData[2] += 16384; //设芯片Z轴竖直向下，设定静态工作点。
}

//算得Roll角。算法见文档。
float SbrXXX04::GetRoll(float *pRealVals, float fNorm)
{
    float fNormXZ = sqrt(pRealVals[0] * pRealVals[0] + pRealVals[2] * pRealVals[2]);
    float fCos = fNormXZ / fNorm;
    return acos(fCos) * fRad2Deg;
}

//算得Pitch角。算法见文档。
float SbrXXX04::GetPitch(float *pRealVals, float fNorm)
{
    float fNormYZ = sqrt(pRealVals[1] * pRealVals[1] + pRealVals[2] * pRealVals[2]);
    float fCos = fNormYZ / fNorm;
    return acos(fCos) * fRad2Deg;
}

//对读数进行纠正，消除偏移，并转换为物理量。公式见文档。
void SbrXXX04::Rectify(int16_t *pReadout, float *pRealVals)
{
    for (int i = 0; i < 3; ++i)
    {
        pRealVals[i] = (float)(pReadout[i] - calibData[i]) / 16384.0f;
    }
    pRealVals[3] = pReadout[3] / 340.0f + 36.53;
    for (int i = 4; i < 7; ++i)
    {
        pRealVals[i] = (float)(pReadout[i] - calibData[i]) / 131.0f;
    }

    Serial.print("Rectified a/g:\t");
    Serial.print(pRealVals[0]);
    Serial.print("\t");
    Serial.print(pRealVals[1]);
    Serial.print("\t");
    Serial.print(pRealVals[2]);
    Serial.print("\t");
    Serial.print(pRealVals[3]);
    Serial.print("\t");
    Serial.print(pRealVals[4]);
    Serial.print("\t");
    Serial.print(pRealVals[5]);
    Serial.print("\t");
    Serial.println(pRealVals[6]);
}