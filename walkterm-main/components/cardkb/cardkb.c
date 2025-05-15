#include "cardkb.h"             //This is the header file for this source file
#include "driver/i2c.h"         //This makes the cardkb comunicate with esp32
#include "esp_log.h"            //This library prints debug log with tags
#include <stdint.h>             //This is for uint data types
#include "freertos/FreeRTOS.h"  //This library is used for vTaskDelay() function

static const char *TAG = "CardKB";      //Tag that is on the beggining of debug from this library in serial monitor

void cardkb_init() {
    ESP_LOGI(TAG, "Initializing cardkb");
    i2c_config_t i2c_config = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    ESP_ERROR_CHECK(i2c_param_config(I2C_MASTER_NUM, &i2c_config));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_MASTER_NUM, I2C_MODE_MASTER, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0x0));
}

uint8_t cardkb_read_key() {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    uint8_t key = 0x0;

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (CARDKB_ADDR << 0x1) | I2C_MASTER_READ, true);
    i2c_master_read_byte(cmd, &key, I2C_MASTER_NACK);
    i2c_master_stop(cmd);

    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(0x3E8));
    i2c_cmd_link_delete(cmd);

    if (ret == ESP_OK) {
        if (key != 0x0) {
            return key;
        }
    } else {
        ESP_LOGE(TAG, "Error reading from CardKB: %s", esp_err_to_name(ret));
    }

    return 0x0;
}

void cardkb_read_word(uint8_t *output, uint8_t size) {
    uint8_t buffer_count = 0x0;
    while (buffer_count < size) {
        uint8_t key = cardkb_read_key();
        if (key != 0x0D || key != 0x20) {
            output[buffer_count++] = key;
        } else {
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(0x2));
    }
    output[buffer_count] = '\0';
}
