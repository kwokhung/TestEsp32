#include <HardwareSerial.h>
#include <Wire.h>
#include <Math.h>

#include "SbrXXX04.h"

void SbrXXX04::setup()
{
    Serial.println("SbrXXX04::setup");

    queueSize = 10;
    queue = xQueueCreate(queueSize, sizeof(int));

    prevTime = 0;     //上一次读数的时间
    prevRoll = 0.0f;  //上一次滤波得到的Roll角
    prevPitch = 0.0f; //上一次滤波得到的Pitch角

    Wire.begin(); //初始化Wire库

    mpu = new MPU6050();

    Serial.println("Initializing I2C devices...");
    mpu->initialize();

    Serial.println("Testing device connections...");
    Serial.println(mpu->testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    calibrate(); //执行校准

    prevTime = micros(); //记录当前时间
}

void SbrXXX04::loop()
{
    Serial.println("SbrXXX04::loop");

    readMpu(); //读出测量值

    rectify(); //根据校准的偏移量进行纠正

    //计算加速度向量的模长，均以g为单位
    float roll = getRoll(); //计算Roll角

    if (calculatedValues[1] > 0)
    {
        roll = -roll;
    }

    float pitch = getPitch(); //计算Pitch角

    if (calculatedValues[0] < 0)
    {
        pitch = -pitch;
    }

    //计算两次测量的时间间隔deltaTime，以秒为单位
    unsigned long curTime = micros();
    float deltaTime = (double)(curTime - prevTime) / 1000000.0;

    //对Roll角和Pitch角进行卡尔曼滤波
    float curRoll = kalmanRoll.getAngle(roll, calculatedValues[4], deltaTime);
    float curPitch = kalmanPitch.getAngle(pitch, calculatedValues[5], deltaTime);

    //跟据滤波值计算角度速
    float rollRate = (curRoll - prevRoll) / deltaTime;
    float pitchRate = (curPitch - prevPitch) / deltaTime;

    //更新Roll角和Pitch角
    prevRoll = curRoll;
    prevPitch = curPitch;

    //更新本次测的时间
    prevTime = curTime;

    //向串口打印输出Roll角和Pitch角，运行时在Arduino的串口监视器中查看
    Serial.print("Roll:");
    Serial.print(curRoll, 6);
    Serial.print('(');
    Serial.print(rollRate, 6);
    Serial.print("),\tPitch:");
    Serial.print(curPitch, 6);
    Serial.print('(');
    Serial.print(pitchRate, 6);
    Serial.print(")\n");

    //sleepAWhile(1000);
    esp_task_wdt_reset();
    delayMicroseconds(10);
}

//从MPU6050读出加速度计三个分量、温度和三个角速度计
//保存在指定的数组中
void SbrXXX04::readMpu()
{
    mpu->getMotion6(&mpuValues[0], &mpuValues[1], &mpuValues[2], &mpuValues[4], &mpuValues[5], &mpuValues[6]);
    mpuValues[3] = mpu->getTemperature();

    Serial.print("a/g:\t");
    Serial.print(mpuValues[0] * 2.0 / 32768, 6);
    Serial.print("\t");
    Serial.print(mpuValues[1] * 2.0 / 32768, 6);
    Serial.print("\t");
    Serial.print(mpuValues[2] * 2.0 / 32768, 6);
    Serial.print("\t");
    Serial.print(mpuValues[3] / 340.00 + 36.53, 6);
    Serial.print("\t");
    Serial.print(mpuValues[4] * 250.0 / 32768, 6);
    Serial.print("\t");
    Serial.print(mpuValues[5] * 250.0 / 32768, 6);
    Serial.print("\t");
    Serial.println(mpuValues[6] * 250.0 / 32768, 6);
}

//对大量读数进行统计，校准平均偏移量
void SbrXXX04::calibrate()
{
    float sumOfMpuValues[7] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

    //先求和
    for (int i = 0; i < 1000; ++i)
    {
        readMpu();

        for (int j = 0; j < 7; ++j)
        {
            sumOfMpuValues[j] += mpuValues[j];
        }
    }

    //再求平均
    for (int i = 0; i < 7; ++i)
    {
        offsetValues[i] = int16_t(sumOfMpuValues[i] / 1000);
    }

    //offsetValues[2] += 16384; //设芯片Z轴竖直向下，设定静态工作点。
}

//对读数进行纠正，消除偏移，并转换为物理量。公式见文档。
void SbrXXX04::rectify()
{
    for (int i = 0; i < 3; ++i)
    {
        calculatedValues[i] = (float)(mpuValues[i] - offsetValues[i]) / 16384.0f;
    }

    calculatedValues[3] = mpuValues[3] / 340.0f + 36.53;

    for (int i = 4; i < 7; ++i)
    {
        calculatedValues[i] = (float)(mpuValues[i] - offsetValues[i]) / 131.0f;
    }

    Serial.print("Rectified a/g:\t");
    Serial.print(calculatedValues[0], 6);
    Serial.print("\t");
    Serial.print(calculatedValues[1], 6);
    Serial.print("\t");
    Serial.print(calculatedValues[2], 6);
    Serial.print("\t");
    Serial.print(calculatedValues[3], 6);
    Serial.print("\t");
    Serial.print(calculatedValues[4], 6);
    Serial.print("\t");
    Serial.print(calculatedValues[5], 6);
    Serial.print("\t");
    Serial.println(calculatedValues[6], 6);
}

//算得Roll角。算法见文档。
float SbrXXX04::getRoll()
{
    float normalInXZ = sqrt(calculatedValues[0] * calculatedValues[0] + calculatedValues[2] * calculatedValues[2]);
    float normal = sqrt(calculatedValues[0] * calculatedValues[0] + calculatedValues[1] * calculatedValues[1] + calculatedValues[2] * calculatedValues[2]);
    float cosine = normalInXZ / normal;

    return acos(cosine) * Rad2Deg;
}

//算得Pitch角。算法见文档。
float SbrXXX04::getPitch()
{
    float normalInYZ = sqrt(calculatedValues[1] * calculatedValues[1] + calculatedValues[2] * calculatedValues[2]);
    float normal = sqrt(calculatedValues[0] * calculatedValues[0] + calculatedValues[1] * calculatedValues[1] + calculatedValues[2] * calculatedValues[2]);
    float cosine = normalInYZ / normal;

    return acos(cosine) * Rad2Deg;
}
