#include "SbrXXX12.hpp"

void SbrXXX12::onDisconnect(BLEServer *bleServer)
{
    Serial.println("SbrXXX12::onDisconnect...");

    connected = false;
}

void SbrXXX12::onConnect(BLEServer *bleServer)
{
    Serial.println("SbrXXX12::onConnect...");

    connected = true;
}

void SbrXXX12::setup()
{
    Serial.println("SbrXXX12::setup");

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

    BLEDevice::init(name);

    bleServer = BLEDevice::createServer();

    bleServer->setCallbacks(this);

    Keyboard::init(bleServer);

    bleServer->getAdvertising()->setAppearance(HID_KEYBOARD);
    bleServer->getAdvertising()->addServiceUUID((uint16_t)0x1812);
    bleServer->getAdvertising()->start();

    Serial.println("SbrXXX12::Characteristic defined! Now you can read it in your phone!");
}

void SbrXXX12::loop()
{
    Serial.println("SbrXXX12::loop");

    int size;

    size = i2c_slave_read_buffer(I2C_EXAMPLE_SLAVE_NUM, data, RW_TEST_LENGTH, 1000 / portTICK_RATE_MS);

    if (size > 0)
    {
        Serial.printf("----I2C input: [%d] bytes ----\n", RW_TEST_LENGTH);
        displayBuffer(data, RW_TEST_LENGTH);

        if (SbrXXX12::isConnected())
        {
            Hid::sendMouse(data[0], data[1], data[2], data[3]);
        }
    }

    sleepAWhile(1000);
}

bool SbrXXX12::isConnected()
{
    return connected;
}

bool SbrXXX12::connected = false;
uint8_t *SbrXXX12::data = (uint8_t *)malloc(DATA_LENGTH);
