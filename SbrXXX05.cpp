#include <Arduino.h>
#include <HardwareSerial.h>
#include <Wire.h>

#include "SbrXXX05.h"

void SbrXXX05::setup()
{
    Serial.println("SbrXXX05::setup");

    queueSize = 10;
    queue = xQueueCreate(queueSize, sizeof(int));

    Wire.begin();

    i2cData[0] = 7;    // Set the sample rate to 1000Hz - 8kHz/(7+1) = 1000Hz
    i2cData[1] = 0x00; // Disable FSYNC and set 260 Hz Acc filtering, 256 Hz Gyro filtering, 8 KHz sampling
    i2cData[2] = 0x00; // Set Gyro Full Scale Range to ±250deg/s
    i2cData[3] = 0x00; // Set Accelerometer Full Scale Range to ±2g

    while (mpuWrite(0x19, i2cData, 4, false))
    {
        // Write to all four registers at once
    }

    while (mpuWrite(0x6B, 0x01, true))
    {
        // PLL with X axis gyroscope reference and disable sleep mode
    }

    while (mpuRead(0x75, i2cData, 1))
    {
    }

    if (i2cData[0] != 0x68)
    {
        // Read "WHO_AM_I" register
        Serial.print(F("Error reading sensor"));

        while (true)
        {
        }
    }

    delayMicroseconds(100); // Wait for sensor to stabilize

    /* Set kalman and gyro starting angle */
    while (mpuRead(0x3B, i2cData, 6))
    {
    }

    accX = (int16_t)((i2cData[0] << 8) | i2cData[1]);
    accY = (int16_t)((i2cData[2] << 8) | i2cData[3]);
    accZ = (int16_t)((i2cData[4] << 8) | i2cData[5]);

    // Source: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf eq. 25 and eq. 26
    // atan2 outputs the value of -π to π (radians) - see http://en.wikipedia.org/wiki/Atan2
    // It is then converted from radians to degrees
#ifdef RESTRICT_PITCH // Eq. 25 and 26
    double roll = atan2(accY, accZ) * RAD_TO_DEG;
    double pitch = atan(-accX / sqrt(accY * accY + accZ * accZ)) * RAD_TO_DEG;
#else // Eq. 28 and 29
    double roll = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;
    double pitch = atan2(-accX, accZ) * RAD_TO_DEG;
#endif

    kalmanX.setAngle(roll); // Set starting angle
    kalmanY.setAngle(pitch);
    gyroXangle = roll;
    gyroYangle = pitch;
    compAngleX = roll;
    compAngleY = pitch;

    timer = micros();
}

void SbrXXX05::loop()
{
    Serial.println("SbrXXX05::loop");

    /* Update all the values */
    while (mpuRead(0x3B, i2cData, 14))
    {
    }

    accX = (int16_t)((i2cData[0] << 8) | i2cData[1]);
    accY = (int16_t)((i2cData[2] << 8) | i2cData[3]);
    accZ = (int16_t)((i2cData[4] << 8) | i2cData[5]);
    tempRaw = (int16_t)((i2cData[6] << 8) | i2cData[7]);
    gyroX = (int16_t)((i2cData[8] << 8) | i2cData[9]);
    gyroY = (int16_t)((i2cData[10] << 8) | i2cData[11]);
    gyroZ = (int16_t)((i2cData[12] << 8) | i2cData[13]);

    double dt = (double)(micros() - timer) / 1000000; // Calculate delta time
    timer = micros();

    // Source: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf eq. 25 and eq. 26
    // atan2 outputs the value of -π to π (radians) - see http://en.wikipedia.org/wiki/Atan2
    // It is then converted from radians to degrees
#ifdef RESTRICT_PITCH // Eq. 25 and 26
    double roll = atan2(accY, accZ) * RAD_TO_DEG;
    double pitch = atan(-accX / sqrt(accY * accY + accZ * accZ)) * RAD_TO_DEG;
#else // Eq. 28 and 29
    double roll = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;
    double pitch = atan2(-accX, accZ) * RAD_TO_DEG;
#endif

    double gyroXrate = gyroX / 131.0; // Convert to deg/s
    double gyroYrate = gyroY / 131.0; // Convert to deg/s

#ifdef RESTRICT_PITCH
    // This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees
    if ((roll < -90 && kalAngleX > 90) || (roll > 90 && kalAngleX < -90))
    {
        kalmanX.setAngle(roll);
        compAngleX = roll;
        kalAngleX = roll;
        gyroXangle = roll;
    }
    else
    {
        kalAngleX = kalmanX.getAngle(roll, gyroXrate, dt); // Calculate the angle using a Kalman filter
    }

    if (abs(kalAngleX) > 90)
    {
        gyroYrate = -gyroYrate; // Invert rate, so it fits the restriced accelerometer reading
    }

    kalAngleY = kalmanY.getAngle(pitch, gyroYrate, dt);
#else
    // This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees
    if ((pitch < -90 && kalAngleY > 90) || (pitch > 90 && kalAngleY < -90))
    {
        kalmanY.setAngle(pitch);
        compAngleY = pitch;
        kalAngleY = pitch;
        gyroYangle = pitch;
    }
    else
    {
        kalAngleY = kalmanY.getAngle(pitch, gyroYrate, dt); // Calculate the angle using a Kalman filter
    }

    if (abs(kalAngleY) > 90)
    {
        gyroXrate = -gyroXrate; // Invert rate, so it fits the restriced accelerometer reading
    }

    kalAngleX = kalmanX.getAngle(roll, gyroXrate, dt); // Calculate the angle using a Kalman filter
#endif

    gyroXangle += gyroXrate * dt; // Calculate gyro angle without any filter
    gyroYangle += gyroYrate * dt;
    //gyroXangle += kalmanX.getRate() * dt; // Calculate gyro angle using the unbiased rate
    //gyroYangle += kalmanY.getRate() * dt;

    compAngleX = 0.93 * (compAngleX + gyroXrate * dt) + 0.07 * roll; // Calculate the angle using a Complimentary filter
    compAngleY = 0.93 * (compAngleY + gyroYrate * dt) + 0.07 * pitch;

    // Reset the gyro angle when it has drifted too much
    if (gyroXangle < -180 || gyroXangle > 180)
    {
        gyroXangle = kalAngleX;
    }

    if (gyroYangle < -180 || gyroYangle > 180)
    {
        gyroYangle = kalAngleY;
    }

    Serial.print(accX);
    Serial.print("\t");
    Serial.print(accY);
    Serial.print("\t");
    Serial.print(accZ);
    Serial.print("\t");

    Serial.print(gyroX);
    Serial.print("\t");
    Serial.print(gyroY);
    Serial.print("\t");
    Serial.print(gyroZ);
    Serial.print("\t");

    Serial.print("\t");

    Serial.print(roll);
    Serial.print("\t");
    Serial.print(gyroXangle);
    Serial.print("\t");
    Serial.print(compAngleX);
    Serial.print("\t");
    Serial.print(kalAngleX);
    Serial.print("\t");

    Serial.print("\t");

    Serial.print(pitch);
    Serial.print("\t");
    Serial.print(gyroYangle);
    Serial.print("\t");
    Serial.print(compAngleY);
    Serial.print("\t");
    Serial.print(kalAngleY);
    Serial.print("\t");

    Serial.print("\t");

    double temperature = (double)tempRaw / 340.0 + 36.53;
    Serial.print(temperature);
    Serial.print("\t");

    Serial.print("\r\n");

    //sleepAWhile(1000);
    esp_task_wdt_reset();
    delayMicroseconds(2);
}

uint8_t SbrXXX05::mpuWrite(uint8_t registerAddress, uint8_t data, bool sendStop)
{
    return mpuWrite(registerAddress, &data, 1, sendStop); // Returns 0 on success
}

uint8_t SbrXXX05::mpuWrite(uint8_t registerAddress, uint8_t *data, uint8_t length, bool sendStop)
{
    Wire.beginTransmission(IMUAddress);
    Wire.write(registerAddress);
    Wire.write(data, length);

    uint8_t rcode = Wire.endTransmission(sendStop); // Returns 0 on success

    if (rcode)
    {
        Serial.print(F("mpuWrite failed: "));
        Serial.println(rcode);
    }

    return rcode; // See: http://arduino.cc/en/Reference/WireEndTransmission
}

uint8_t SbrXXX05::mpuRead(uint8_t registerAddress, uint8_t *data, uint8_t nbytes)
{
    uint32_t timeOutTimer;

    Wire.beginTransmission(IMUAddress);
    Wire.write(registerAddress);

    uint8_t rcode = Wire.endTransmission(false); // Don't release the bus

    if (rcode)
    {
        Serial.print(F("mpuRead failed: "));
        Serial.println(rcode);

        return rcode;
    }

    Wire.requestFrom(IMUAddress, nbytes, (uint8_t) true); // Send a repeated start and then release the bus after reading

    for (uint8_t i = 0; i < nbytes; i++)
    {
        if (Wire.available())
        {
            data[i] = Wire.read();
        }
        else
        {
            timeOutTimer = micros();

            while (((micros() - timeOutTimer) < I2C_TIMEOUT) && !Wire.available())
            {
            }

            if (Wire.available())
            {
                data[i] = Wire.read();
            }
            else
            {
                Serial.println(F("mpuRead timeout"));

                return 5; // This error value is not already taken by endTransmission
            }
        }
    }

    return 0; // Success
}
