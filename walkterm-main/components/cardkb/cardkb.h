/*
Written by Jaroslav Vizner in January 2025
Don't change anything if you don't know what are you doing.
*/

#ifndef __CARDKB_H__
#define __CARDKB_H__

#include <stdint.h>
#define I2C_MASTER_SCL_IO    CONFIG_I2C_MASTER_SCL_IO       // I2C SCL pin
#define I2C_MASTER_SDA_IO    CONFIG_I2C_MASTER_SDA_IO       // I2C SDA pin
#define I2C_MASTER_NUM       I2C_NUM_0                      // I2C master number
#define I2C_MASTER_FREQ_HZ   CONFIG_I2C_MASTER_FREQ_HZ      // Frequency of I2C bus
#define I2C_MASTER_TX_BUF_DISABLE 0x0
#define I2C_MASTER_RX_BUF_DISABLE 0x0
#define CARDKB_ADDR          CONFIG_CARDKB_ADDR             // I2C address of cardkb

void cardkb_init();                                         // function to initialize cardkb using config from menuconfig
uint8_t cardkb_read_key();                                  // function to read key that is pressed at the moment
void cardkb_read_word(uint8_t *output, uint8_t size);       // function to read word untill enter or space or reached maximum size of word

#endif
