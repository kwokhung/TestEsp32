#ifndef SbrXXX05_h
#define SbrXXX05_h

#include <Kalman.h>

#include "SbrBase.h"

#define RESTRICT_PITCH // Comment out to restrict roll to ±90deg instead - please read: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf
#define IMUAddress 0x68
#define I2C_TIMEOUT 1000

class SbrXXX05 : public SbrBase<SbrXXX05>
{
public:
  friend class SbrBase;
  uint8_t i2cWrite(uint8_t registerAddress, uint8_t data, bool sendStop);
  uint8_t i2cWrite(uint8_t registerAddress, uint8_t *data, uint8_t length, bool sendStop);
  uint8_t i2cRead(uint8_t registerAddress, uint8_t *data, uint8_t nbytes);

private:
  SbrXXX05(std::string name)
      : SbrBase(name)
  {
  }
  void setup() override;
  void loop() override;

  Kalman kalmanX; // Create the Kalman instances
  Kalman kalmanY;

  /* IMU Data */
  double accX, accY, accZ;
  double gyroX, gyroY, gyroZ;
  int16_t tempRaw;

  double gyroXangle, gyroYangle; // Angle calculate using the gyro only
  double compAngleX, compAngleY; // Calculated angle using a complementary filter
  double kalAngleX, kalAngleY;   // Calculated angle using a Kalman filter

  uint32_t timer;
  uint8_t i2cData[14]; // Buffer for I2C data
};

#endif