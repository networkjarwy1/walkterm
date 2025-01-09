#ifndef __ADDRESS_SQUARED_BUS__
#define __ADDRESS_SQUARED_BUS__

#include <stdio.h>
#include "addsqrtbusStdCharTable.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define CS      0x5
#define DATA    0xC
#define CLK     0xD

bool CLK_state = 0x0;

void bus_init(){
    gpio_set_direction(CS, GPIO_MODE_OUTPUT);
    gpio_set_direction(DATA, GPIO_MODE_OUTPUT);
    gpio_set_direction(CLK, GPIO_MODE_OUTPUT);
    gpio_set_level(CS, 0x0);
    gpio_set_level(DATA, 0x0);
    gpio_set_level(CLK, 0x0);
}

uint8_t findChar(char c){
    for(uint8_t i = 0x0; i < charTable_size; i++){
        if(c == charTable[i]){
            return i;
        }
    }

    printf("error: char isn't in standard char table");
    return 0xFF;
}

void uint8_to_binary_array(uint8_t value, uint8_t binary_array[0x8]) {
    for (uint8_t i = 0x0; i < 0x8; i++) {
        binary_array[0x7 - i] = (value >> i) & 0x1;
    }
}

void sendChar(char c){
    uint8_t binArr[0x8];
    uint8_to_binary_array(findChar(c), binArr);
    gpio_set_level(CS, 0x1);
    vTaskDelay(pdMS_TO_TICKS(0x2));

    for(uint8_t i = 0x0; i < 0x8; i++){
        gpio_set_level(DATA, binArr[i]);
        vTaskDelay(pdMS_TO_TICKS(0x1));
        gpio_set_level(CLK, !CLK_state);
        CLK_state = !CLK_state;
    }
    gpio_set_direction(CS, 0x0);
}

#endif