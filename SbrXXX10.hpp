#pragma once

#include <Arduino.h>
#include <driver/i2c.h>

#include "SbrXXX08.hpp"
#include "SbrBase.hpp"

#define DATA_LENGTH 512   /*!<Data buffer length for test buffer*/
#define RW_TEST_LENGTH 10 /*!<Data length for r/w test, any value from 0-DATA_LENGTH*/

#define I2C_EXAMPLE_SLAVE_SCL_IO GPIO_NUM_22           /*!<gpio number for i2c slave clock  */
#define I2C_EXAMPLE_SLAVE_SDA_IO GPIO_NUM_21           /*!<gpio number for i2c slave data */
#define I2C_EXAMPLE_SLAVE_NUM I2C_NUM_0                /*!<I2C port number for slave dev */
#define I2C_EXAMPLE_SLAVE_TX_BUF_LEN (2 * DATA_LENGTH) /*!<I2C slave tx buffer size */
#define I2C_EXAMPLE_SLAVE_RX_BUF_LEN (2 * DATA_LENGTH) /*!<I2C slave rx buffer size */

#define ESP_SLAVE_ADDR 0x28 /*!< ESP32 slave address, you can set any 7bit value */

class SbrXXX10 : public SbrBase<SbrXXX10>
{
public:
  friend class SbrBase;

  static SbrXXX08 *sbrXXX08;

private:
  SbrXXX10(std::string name)
      : SbrBase(name)
  {
  }
  void setup() override;
  void loop() override;
};
