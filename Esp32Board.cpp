#include "Esp32Board.hpp"

Esp32Board::Esp32Board()
{
}

void Esp32Board::init(void)
{
    Wire.begin();

    mpu = new MPU6050();

    Serial.println("Initializing I2C devices...");
    mpu->initialize();

    Serial.println("Testing device connections...");
    Serial.println(mpu->testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
}

bool Esp32Board::getEulerAngles(float eulerAngles[3])
{
    return true;
}

bool Esp32Board::getGyroRates(float gyroRates[3])
{
    return true;
}

uint32_t Esp32Board::getMicroseconds()
{
    return 0;
}

void Esp32Board::writeMotor(uint8_t index, float value)
{
}

bool Esp32Board::getAccelerometer(float accelGs[3])
{
    readMpu();

    accelGs[0] = mpuValues[0];
    accelGs[1] = mpuValues[1];
    accelGs[2] = mpuValues[2];
    
    return true;
}

void Esp32Board::readMpu()
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

void hf::Board::outbuf(char *buf)
{
    Serial.print(buf);
}
