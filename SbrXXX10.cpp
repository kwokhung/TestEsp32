#include "SbrXXX10.hpp"

void SbrXXX10::setup()
{
    Serial.println("SbrXXX10::setup");

    queueSize = 10;
    queue = xQueueCreate(queueSize, sizeof(int));

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

void SbrXXX10::loop()
{
    //Serial.println("SbrXXX10::loop");

    int size;

    size = i2c_slave_read_buffer(I2C_EXAMPLE_SLAVE_NUM, data, RW_TEST_LENGTH, 1000 / portTICK_RATE_MS);

    if (size > 0)
    {
        xQueueSend(sbrXXX11->queue, data, portMAX_DELAY);
    }

    sleepAWhile(1000);
}

SbrXXX11 *SbrXXX10::sbrXXX11;
uint8_t *SbrXXX10::data = (uint8_t *)malloc(DATA_LENGTH);
