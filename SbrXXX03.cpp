#include <HardwareSerial.h>

#include "SbrXXX03.h"

void SbrXXX03::setup()
{
    Serial.println("SbrXXX03::setup");

    queueSize = 10;
    queue = xQueueCreate(queueSize, sizeof(int));

    accelgyro = new MPU6050();

// join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
#endif

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro->initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro->testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    Serial.println("Updating internal sensor offsets...");
    Serial.print(accelgyro->getXAccelOffset());
    Serial.print("\t");
    Serial.print(accelgyro->getYAccelOffset());
    Serial.print("\t");
    Serial.print(accelgyro->getZAccelOffset());
    Serial.print("\t");
    Serial.print(accelgyro->getXGyroOffset());
    Serial.print("\t");
    Serial.print(accelgyro->getYGyroOffset());
    Serial.print("\t");
    Serial.print(accelgyro->getZGyroOffset());
    Serial.print("\t");
    Serial.print("\n");

    accelgyro->setXAccelOffset(0);
    accelgyro->setYAccelOffset(0);
    accelgyro->setZAccelOffset(0);

    Serial.print(accelgyro->getXAccelOffset());
    Serial.print("\t");
    Serial.print(accelgyro->getYAccelOffset());
    Serial.print("\t");
    Serial.print(accelgyro->getZAccelOffset());
    Serial.print("\t");
    Serial.print(accelgyro->getXGyroOffset());
    Serial.print("\t");
    Serial.print(accelgyro->getYGyroOffset());
    Serial.print("\t");
    Serial.print(accelgyro->getZGyroOffset());
    Serial.print("\t");
    Serial.print("\n");
}

void SbrXXX03::loop()
{
    Serial.println("SbrXXX03::loop");

    sleepAWhile(1000);
}
