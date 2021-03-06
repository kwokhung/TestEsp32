#include "SbrXXX06.h"

#include "Esp32Board.hpp"

void SbrXXX06::setup()
{
    Serial.println("SbrXXX06::setup");

    queueSize = 10;
    queue = xQueueCreate(queueSize, sizeof(int));

    h.init(new Esp32Board(), &rc, &stabilizer);
}

void SbrXXX06::loop()
{
    Serial.println("SbrXXX06::loop");

    h.update();

    sleepAWhile(1000);
}

hf::Hackflight &SbrXXX06::h = *new hf::Hackflight();
hf::SBUS_Receiver &SbrXXX06::rc = *new hf::SBUS_Receiver(.005f, -.08f, 0.f);
hf::Stabilizer &SbrXXX06::stabilizer = *new hf::Stabilizer(
    0.20f,      // Level P
    0.225f,     // Gyro cyclic P
    0.001875f,  // Gyro cyclic I
    0.375f,     // Gyro cyclic D
    1.0625f,    // Gyro yaw P
    0.005625f); // Gyro yaw I
