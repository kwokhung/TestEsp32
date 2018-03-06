#include <HardwareSerial.h>

#include "SbrXXX03.h"

void SbrXXX03::setup()
{
    Serial.println("SbrXXX03::setup");

    queueSize = 10;
    queue = xQueueCreate(queueSize, sizeof(int));

    Wire.begin();

    accelgyro = new MPU6050();

    Serial.println("Initializing I2C devices...");
    accelgyro->initialize();

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

    /*accelgyro->setXAccelOffset(0);
    accelgyro->setYAccelOffset(90);
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
    Serial.print("\n");*/
}

void SbrXXX03::loop()
{
    Serial.println("SbrXXX03::loop");

    accelgyro->getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    Serial.print("a/g:\t");
    Serial.print(ax);
    Serial.print("\t");
    Serial.print(ay);
    Serial.print("\t");
    Serial.print(az);
    Serial.print("\t");
    Serial.print(gx);
    Serial.print("\t");
    Serial.print(gy);
    Serial.print("\t");
    Serial.println(gz);

    axInG = ax * 2.0 / 32768;
    ayInG = ay * 2.0 / 32768;
    azInG = az * 2.0 / 32768;

    Serial.print("a in g:\t");
    Serial.print(axInG);
    Serial.print("\t");
    Serial.print(ayInG);
    Serial.print("\t");
    Serial.println(azInG);

    sleepAWhile(1000);
}
