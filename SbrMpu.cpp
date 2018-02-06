#include <HardwareSerial.h>
#include <Wire.h>

#include "SbrMpu.h"

SbrMpu::SbrMpu(std::string name)
    : name(name)
{
    task = NULL;
}

void SbrMpu::setup()
{
    Serial.println("SbrMpu::setup");

    setup_mpu();
}

void SbrMpu::setup_mpu()
{
    Wire.begin();
    Wire.setClock(400000L);

    //By default the MPU-6050 sleeps. So we have to wake it up.
    Wire.beginTransmission(SbrMpu::MPU_ADDR);
    Wire.write(0x6B); //We want to write to the PWR_MGMT_1 register (6B hex)
    Wire.write(0x00); //Set the register bits as 00000000 to activate the gyro
    Wire.endTransmission();

    Wire.beginTransmission(SbrMpu::MPU_ADDR);
    Wire.write(0x1B); //We want to write to the GYRO_CONFIG register (1B hex)
    Wire.write(GYRO_FULL_SCALE_RANGE);
    Wire.endTransmission();

    Wire.beginTransmission(SbrMpu::MPU_ADDR);
    Wire.write(0x1C); //We want to write to the ACCEL_CONFIG register (1A hex)
    Wire.write(ACC_FULL_SCALE_RANGE);
    Wire.endTransmission();

    //Set some filtering to improve the raw data.
    Wire.beginTransmission(SbrMpu::MPU_ADDR);
    Wire.write(0x1A); //We want to write to the CONFIG register (1A hex)
    Wire.write(0x03); //Set the register bits as 00000011 (Set Digital Low Pass Filter to ~43Hz)
    Wire.endTransmission();

    calibrateGyro();
}

void SbrMpu::calibrateGyro()
{
    int32_t x, y, z;

    for (int i = 0; i < 500; i++)
    {
        getRotation(&gyroX, &gyroY, &gyroZ);

        x += gyroX;
        y += gyroY;
        z += gyroZ;

        delayMicroseconds(PERIOD); // simulate the main program loop time ??
    }

    gyroX_calibration = x / 500;
    gyroY_calibration = y / 500;
    gyroZ_calibration = z / 500;
}

void SbrMpu::getAcceleration(int16_t *x, int16_t *y, int16_t *z)
{
    Wire.beginTransmission(SbrMpu::MPU_ADDR);
    Wire.write(ACCEL_XOUT_H);
    Wire.endTransmission();
    Wire.requestFrom(SbrMpu::MPU_ADDR, 6);

    *x = SbrMpu::constr((((int16_t)Wire.read()) << 8) | Wire.read(), -ACC_SCALE_FACTOR, ACC_SCALE_FACTOR);
    *y = SbrMpu::constr((((int16_t)Wire.read()) << 8) | Wire.read(), -ACC_SCALE_FACTOR, ACC_SCALE_FACTOR);
    *z = SbrMpu::constr((((int16_t)Wire.read()) << 8) | Wire.read(), -ACC_SCALE_FACTOR, ACC_SCALE_FACTOR);
}

void SbrMpu::getRotation(int16_t *x, int16_t *y, int16_t *z)
{
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(GYRO_XOUT_H);
    Wire.endTransmission();
    Wire.requestFrom(MPU_ADDR, 6);
    
    *x = ((((int16_t)Wire.read()) << 8) | Wire.read()) - gyroX_calibration;
    *y = ((((int16_t)Wire.read()) << 8) | Wire.read()) - gyroY_calibration;
    *z = ((((int16_t)Wire.read()) << 8) | Wire.read()) - gyroZ_calibration;
}

void SbrMpu::startUp(void *parameter)
{
    SbrMpu *sbrMpu = (SbrMpu *)parameter;

    sbrMpu->setup();

    while (true)
    {
    }

    vTaskDelete(NULL);
}

int16_t SbrMpu::constr(int16_t value, int16_t mini, int16_t maxi)
{
    if (value < mini)
    {
        return mini;
    }
    else if (value > maxi)
    {
        return maxi;
    }

    return value;
}

float SbrMpu::constrf(float value, float mini, float maxi)
{
    if (value < mini)
    {
        return mini;
    }
    else if (value > maxi)
    {
        return maxi;
    }

    return value;
}

int SbrMpu::MPU_ADDR = 0x69; //AD0 is HIGH
float SbrMpu::GYRO_RAW_TO_DEGS = 1.0 / (1000000.0 / PERIOD) / GYRO_SCALE_FACTOR;
