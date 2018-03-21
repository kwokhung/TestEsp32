#include "SbrXXX06.h"

void SbrXXX06::setup()
{
    Serial.println("SbrXXX06::setup");

    queueSize = 10;
    queue = xQueueCreate(queueSize, sizeof(int));

    rc = *new hf::SBUS_Receiver(.005f, -.08f, 0.f);
    stabilizer = *new hf::Stabilizer(
        0.20f,      // Level P
        0.225f,     // Gyro cyclic P
        0.001875f,  // Gyro cyclic I
        0.375f,     // Gyro cyclic D
        1.0625f,    // Gyro yaw P
        0.005625f); // Gyro yaw I

    h.init(new hf::SimBoard(), &rc, &stabilizer);
}

void SbrXXX06::loop()
{
    Serial.println("SbrXXX06::loop");

    sleepAWhile(1000);
}
