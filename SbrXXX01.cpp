#include <Arduino.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <esp_log.h>

#include "SbrXXX01.h"

uint8_t SbrXXX01::readMPU6050(uint8_t reg)
{
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(reg);
    Wire.endTransmission(true);

    Wire.requestFrom(MPU6050_ADDR, 1 /*length*/);

    return Wire.read();
}

void SbrXXX01::writeMPU6050(uint8_t reg, uint8_t data)
{
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(reg);
    Wire.write(data);
    Wire.endTransmission();
}

void SbrXXX01::calcRotation()
{
    int16_t raw_acc_x, raw_acc_y, raw_acc_z, raw_t, raw_gyro_x, raw_gyro_y, raw_gyro_z;

    //レジスタアドレス0x3Bから、計14バイト分のデータを出力するようMPU6050へ指示
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050_ADDR, 14, true);

    //出力されたデータを読み込み、ビットシフト演算
    raw_acc_x = Wire.read() << 8 | Wire.read();
    raw_acc_y = Wire.read() << 8 | Wire.read();
    raw_acc_z = Wire.read() << 8 | Wire.read();
    raw_t = Wire.read() << 8 | Wire.read();
    raw_gyro_x = Wire.read() << 8 | Wire.read();
    raw_gyro_y = Wire.read() << 8 | Wire.read();
    raw_gyro_z = Wire.read() << 8 | Wire.read();

    //単位Gへ変換
    acc_x = ((float)raw_acc_x) / 16384.0;
    acc_y = ((float)raw_acc_y) / 16384.0;
    acc_z = ((float)raw_acc_z) / 16384.0;

    //加速度センサーから角度を算出
    acc_angle_y = atan2(acc_x, acc_z + abs(acc_y)) * 360 / -2.0 / PI;
    acc_angle_x = atan2(acc_y, acc_z + abs(acc_x)) * 360 / 2.0 / PI;

    dpsX = ((float)raw_gyro_x) / 65.5; // LSB sensitivity: 65.5 LSB/dps @ ±500dps
    dpsY = ((float)raw_gyro_y) / 65.5;
    dpsZ = ((float)raw_gyro_z) / 65.5;

    //前回計算した時から今までの経過時間を算出
    interval = millis() - preInterval;
    preInterval = millis();

    //数値積分
    gyro_angle_x += (dpsX - offsetX) * (interval * 0.001);
    gyro_angle_y += (dpsY - offsetY) * (interval * 0.001);
    gyro_angle_z += (dpsZ - offsetZ) * (interval * 0.001);

    //相補フィルター
    angleX = (0.996 * gyro_angle_x) + (0.004 * acc_angle_x);
    angleY = (0.996 * gyro_angle_y) + (0.004 * acc_angle_y);
    angleZ = gyro_angle_z;

    gyro_angle_x = angleX;
    gyro_angle_y = angleY;
    gyro_angle_z = angleZ;
}

void SbrXXX01::setup()
{
    Serial.println("SbrXXX01::setup");

    queueSize = 10;
    queue = xQueueCreate(queueSize, sizeof(int));

    offsetX = 0;
    offsetY = 0;
    offsetZ = 0;

    gyro_angle_x = 0;
    gyro_angle_y = 0;
    gyro_angle_z = 0;

    //Wire.begin();
    Wire.begin(SDAPIN, SCLPIN);

    writeMPU6050(MPU6050_PWR_MGMT_1, 0x00);

    sleepAWhile(100);

    if (readMPU6050(MPU6050_WHO_AM_I) != MPU6050_ADDR)
    {
        Serial.println("\nWHO_AM_I error.");

        while (true)
        {
            sleepAWhile(1000);
        }
    }

    writeMPU6050(MPU6050_SMPLRT_DIV, 0x00);   // sample rate: 8kHz/(7+1) = 1kHz
    writeMPU6050(MPU6050_CONFIG, 0x00);       // disable DLPF, gyro output rate = 8kHz
    writeMPU6050(MPU6050_GYRO_CONFIG, 0x08);  // gyro range: ±500dps
    writeMPU6050(MPU6050_ACCEL_CONFIG, 0x00); // accel range: ±2g
    writeMPU6050(MPU6050_PWR_MGMT_1, 0x01);   // disable sleep mode, PLL with X gyro

    Serial.print("Calculate Calibration");

    for (int i = 0; i < 3000; i++)
    {
        int16_t raw_acc_x, raw_acc_y, raw_acc_z, raw_t, raw_gyro_x, raw_gyro_y, raw_gyro_z;

        Wire.beginTransmission(MPU6050_ADDR);
        Wire.write(0x3B);
        Wire.endTransmission(false);
        Wire.requestFrom(MPU6050_ADDR, 14, true);

        raw_acc_x = Wire.read() << 8 | Wire.read();
        raw_acc_y = Wire.read() << 8 | Wire.read();
        raw_acc_z = Wire.read() << 8 | Wire.read();
        raw_t = Wire.read() << 8 | Wire.read();
        raw_gyro_x = Wire.read() << 8 | Wire.read();
        raw_gyro_y = Wire.read() << 8 | Wire.read();
        raw_gyro_z = Wire.read() << 8 | Wire.read();

        Serial.print("AcX = ");
        Serial.print(raw_acc_x);
        Serial.print(" | AcY = ");
        Serial.print(raw_acc_y);
        Serial.print(" | AcZ = ");
        Serial.print(raw_acc_z);
        Serial.print(" | Tmp = ");
        Serial.print(raw_t / 340.00 + 36.53); //equation for temperature in degrees C from datasheet
        Serial.print(" | GyX = ");
        Serial.print(raw_gyro_x);
        Serial.print(" | GyY = ");
        Serial.print(raw_gyro_y);
        Serial.print(" | GyZ = ");
        Serial.println(raw_gyro_z);

        dpsX = ((float)raw_gyro_x) / 65.5;
        dpsY = ((float)raw_gyro_y) / 65.5;
        dpsZ = ((float)raw_gyro_z) / 65.5;

        offsetX += dpsX;
        offsetY += dpsY;
        offsetZ += dpsZ;

        if (i % 1000 == 0)
        {
            Serial.print(".");
        }
    }

    Serial.println();

    offsetX /= 3000;
    offsetY /= 3000;
    offsetZ /= 3000;

    Serial.print("offsetX : ");
    Serial.println(offsetX);
    Serial.print("offsetY : ");
    Serial.println(offsetY);
    Serial.print("offsetZ : ");
    Serial.println(offsetZ);
}

void SbrXXX01::loop()
{
    Serial.println("SbrXXX01::loop");

    /*Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050_ADDR, 14, true); // request a total of 14 registers

    AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
    AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    Tmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    GyX = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    GyY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

    Serial.print("AcX = ");
    Serial.print(AcX);
    Serial.print(" | AcY = ");
    Serial.print(AcY);
    Serial.print(" | AcZ = ");
    Serial.print(AcZ);
    Serial.print(" | Tmp = ");
    Serial.print(Tmp / 340.00 + 36.53); //equation for temperature in degrees C from datasheet
    Serial.print(" | GyX = ");
    Serial.print(GyX);
    Serial.print(" | GyY = ");
    Serial.print(GyY);
    Serial.print(" | GyZ = ");
    Serial.println(GyZ);*/

    calcRotation();

    Serial.print("angleX : ");
    Serial.print(angleX);
    Serial.print("    angleY : ");
    Serial.print(angleY);
    Serial.print("    angleZ : ");
    Serial.println(angleZ);
    
    sleepAWhile(1000);
}
