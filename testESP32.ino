#include <stdio.h>
#include <FreeRTOS.h>
#include <esp_task_wdt.h>
#include <Wire.h>

#define DATA_LENGTH 512  /*!<Data buffer length for test buffer*/
#define RW_TEST_LENGTH 4 /*!<Data length for r/w test, any value from 0-DATA_LENGTH*/

#define ESP_SLAVE_ADDR 0x28 /*!< ESP32 slave address, you can set any 7bit value */

static void i2c_example_master_init()
{
  Wire.begin();
}

static void i2c_example_master_write_slave(uint8_t *data_wr, size_t size)
{
  Wire.beginTransmission(ESP_SLAVE_ADDR);
  Wire.write(data_wr, size);
  Wire.endTransmission(true);
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

  i2c_example_master_init();

  Serial.println("i2c_example_master_init");
}

void loop()
{
  int ret;
  uint8_t *data_wr = (uint8_t *)malloc(DATA_LENGTH);

  data_wr[0] = 0x00;
  data_wr[1] = 0x01;
  data_wr[2] = 0x01;
  data_wr[3] = 0x00;

  while (true)
  {
    i2c_example_master_write_slave(data_wr, RW_TEST_LENGTH);

    printf("*******************\n");
    printf("MASTER WRITE TO SLAVE\n");
    printf("*******************\n");
    printf("----Master write ----\n");

    disp_buf(data_wr, RW_TEST_LENGTH);

    for (int i = 0; i < 1; i++)
    {
      esp_task_wdt_reset();

      delay(1000);
    }
  }
}
