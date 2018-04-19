#include "Esp32Board.hpp"

Esp32Board::Esp32Board()
{
    etk::Quaternion A, B;

    //heading is 90, zero pitch or roll
    etk::Vector<3> a(0, 0, 90);
    a.to_radians();

    //heading is 45
    etk::Vector<3> b(0, 0, 45);
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

    etk::Quaternion q;
    euler = *new etk::Vector<3>(285, 2.5, 1.54);
    euler.to_radians();
    q.from_euler(euler); //this is the orientation quaternion

    //total acceleration is force felt by accelerometers in the aircraft
    etk::Vector<3> total_acceleration(1.5, 0.2, 10.5);
    //gravity makes things feel like they are accelerating up at 9.8m/s/s on the vertical (z) axis
    etk::Vector<3> gravity(0.0, 0.0, 9.8);

    //rotate gravity so it's in the pilots frame of reference
    auto gravity_pilot_frame = q.rotate_vector(gravity);

    //now we can subtract gravity from total acceleration
    etk::Vector<3> acceleration = total_acceleration - gravity_pilot_frame;

    Serial.print(acceleration.x(), 6);
    Serial.print("\t");
    Serial.print(acceleration.y(), 6);
    Serial.print("\t");
    Serial.println(acceleration.z(), 6);

    //Vector<3> down = accelerometers.read();
    //Vector<3> east = down.cross(magnetometers.read());
    //Vector<3> north = east.cross(down);
    etk::Vector<3> down = *new etk::Vector<3>(0, 0, 1);
    etk::Vector<3> east = down.cross(*new etk::Vector<3>(0, 1, 0));
    etk::Vector<3> north = east.cross(down);

    down.normalize();
    east.normalize();
    north.normalize();

    Serial.print(down.x(), 6);
    Serial.print("\t");
    Serial.print(down.y(), 6);
    Serial.print("\t");
    Serial.println(down.z(), 6);

    Serial.print(east.x(), 6);
    Serial.print("\t");
    Serial.print(east.y(), 6);
    Serial.print("\t");
    Serial.println(east.z(), 6);

    Serial.print(north.x(), 6);
    Serial.print("\t");
    Serial.print(north.y(), 6);
    Serial.print("\t");
    Serial.println(north.z(), 6);

    etk::Matrix<3, 3> rotationMatrix;
    rotationMatrix.vector_to_col(north, 0);
    rotationMatrix.vector_to_col(east, 1);
    rotationMatrix.vector_to_col(down, 2);

    etk::Quaternion q_accel;
    q_accel.from_matrix(rotationMatrix);

    euler = q_accel.to_euler();
    euler.to_degrees();

    Serial.print(euler.x(), 6);
    Serial.print("\t");
    Serial.print(euler.y(), 6);
    Serial.print("\t");
    Serial.println(euler.z(), 6);

    euler.x() = atan2f(1, 0);
    euler.y() = atan2(-0, sqrt(0 * 0 + 1 * 1));
    euler.z() = atan2(0, 1);

    Serial.print(euler.x(), 6);
    Serial.print("\t");
    Serial.print(euler.y(), 6);
    Serial.print("\t");
    Serial.println(euler.z(), 6);

    q_accel.from_euler(euler);

    euler = q_accel.to_euler();
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
