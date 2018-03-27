#include "Esp32Board.hpp"

Esp32Board::Esp32Board()
{
    etk::Quaternion A, B;

    //heading is 90, zero pitch or roll
    etk::Vector<3> a(90, 0, 0);
    a.to_radians();

    //heading is 45
    etk::Vector<3> b(45, 0, 0);
    b.to_radians();

    //make quaternions A and B from euler angle representations
    A.from_euler(a);
    B.from_euler(b);

    //multiply A by B
    etk::Quaternion C = A * B;

    //convert C to an euler angle, because it's easier for mere mortals to comprehend
    etk::Vector<3> euler = C.to_euler();
    euler.to_degrees();

    Serial.print(euler.x(), 6);
    Serial.print("\t");
    Serial.print(euler.y(), 6);
    Serial.print("\t");
    Serial.println(euler.z(), 6);
}

void Esp32Board::init(void)
{
    Wire.begin();

    mpu = new MPU6050();

    Serial.println("Initializing MPU6050...");
    mpu->initialize();

    Serial.println("Testing MPU6050 connection...");
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
    return micros();
}

void Esp32Board::writeMotor(uint8_t index, float value)
{
}

bool Esp32Board::getAccelerometer(float accelGs[3])
{
    readMpu();

    accelGs[0] = mpuValues[0] * 2.0 / 32768;
    accelGs[1] = mpuValues[1] * 2.0 / 32768;
    accelGs[2] = mpuValues[2] * 2.0 / 32768;

    return true;
}

void Esp32Board::readMpu()
{
    mpu->getMotion6(&mpuValues[0], &mpuValues[1], &mpuValues[2], &mpuValues[4], &mpuValues[5], &mpuValues[6]);
    mpuValues[3] = mpu->getTemperature();

    /*
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
    */
}

void hf::Board::outbuf(char *buf)
{
    Serial.print(buf);
}
