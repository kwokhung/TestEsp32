#include <stdio.h>
#include <FreeRTOS.h>
#include <esp_task_wdt.h>
#include <driver/i2c.h>

#define DATA_LENGTH 512   /*!<Data buffer length for test buffer*/
#define RW_TEST_LENGTH 10 /*!<Data length for r/w test, any value from 0-DATA_LENGTH*/

#define I2C_EXAMPLE_SLAVE_SCL_IO GPIO_NUM_22           /*!<gpio number for i2c slave clock  */
#define I2C_EXAMPLE_SLAVE_SDA_IO GPIO_NUM_21           /*!<gpio number for i2c slave data */
#define I2C_EXAMPLE_SLAVE_NUM I2C_NUM_0                /*!<I2C port number for slave dev */
#define I2C_EXAMPLE_SLAVE_TX_BUF_LEN (2 * DATA_LENGTH) /*!<I2C slave tx buffer size */
#define I2C_EXAMPLE_SLAVE_RX_BUF_LEN (2 * DATA_LENGTH) /*!<I2C slave rx buffer size */

#define ESP_SLAVE_ADDR 0x28 /*!< ESP32 slave address, you can set any 7bit value */

void i2c_example_slave_init()
{
  i2c_port_t i2c_slave_port = I2C_EXAMPLE_SLAVE_NUM;
  i2c_config_t conf_slave;

  conf_slave.sda_io_num = I2C_EXAMPLE_SLAVE_SDA_IO;
  conf_slave.sda_pullup_en = GPIO_PULLUP_ENABLE;
  conf_slave.scl_io_num = I2C_EXAMPLE_SLAVE_SCL_IO;
  conf_slave.scl_pullup_en = GPIO_PULLUP_ENABLE;
  conf_slave.mode = I2C_MODE_SLAVE;
  conf_slave.slave.addr_10bit_en = 0;
  conf_slave.slave.slave_addr = ESP_SLAVE_ADDR;

  i2c_param_config(i2c_slave_port, &conf_slave);

  i2c_driver_install(i2c_slave_port, conf_slave.mode,
                     I2C_EXAMPLE_SLAVE_RX_BUF_LEN,
                     I2C_EXAMPLE_SLAVE_TX_BUF_LEN, 0);
}

static void disp_buf(uint8_t *buf, int len)
{
  int i;

  for (i = 0; i < len; i++)
  {
    printf("%02x ", buf[i]);

    if ((i + 1) % 16 == 0)
    {
      printf("\n");
    }
  }

  printf("\n");
}

void setup()
{
  Serial.begin(115200);

  i2c_example_slave_init();

  Serial.println("i2c_example_slave_init");
}

void loop()
{
  int size;
  uint8_t *data = (uint8_t *)malloc(DATA_LENGTH);

  while (true)
  {
    size = i2c_slave_read_buffer(I2C_EXAMPLE_SLAVE_NUM, data, RW_TEST_LENGTH, 1000 / portTICK_RATE_MS);

    if (size > 0)
    {
      printf("*******************\n");
      printf("----Slave read: [%d] bytes ----\n", size);

      disp_buf(data, size);
    }
  }
}
