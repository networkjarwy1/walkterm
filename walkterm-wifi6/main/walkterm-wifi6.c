#include <stdio.h>
#include "driver/i2c.h"
#include "esp_log.h"

#define I2C_SLAVE_ADDR 0x08
#define I2C_SLAVE_SDA 21
#define I2C_SLAVE_SCL 22
#define I2C_SLAVE_BUF_LEN 128

void i2c_slave_init() {
    i2c_config_t conf = {
        .mode = I2C_MODE_SLAVE,
        .sda_io_num = I2C_SLAVE_SDA,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_SLAVE_SCL,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .slave.addr_10bit_en = 0,
        .slave.slave_addr = I2C_SLAVE_ADDR,
    };
    i2c_param_config(I2C_NUM_0, &conf);
    i2c_driver_install(I2C_NUM_0, conf.mode, I2C_SLAVE_BUF_LEN, I2C_SLAVE_BUF_LEN, 0);
}

void i2c_slave_task() {
    uint8_t data[I2C_SLAVE_BUF_LEN];
    while (1) {
        int len = i2c_slave_read_buffer(I2C_NUM_0, data, I2C_SLAVE_BUF_LEN, portMAX_DELAY);
        if (len > 0) {
            data[len] = 0;
            printf("Received: %s\n", data);
        }
    }
}

void app_main() {
    i2c_slave_init();
    xTaskCreate(i2c_slave_task, "i2c_slave_task", 2048, NULL, 10, NULL);
}
