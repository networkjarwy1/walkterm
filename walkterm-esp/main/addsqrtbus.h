#ifndef __ADDRESS_SQUARED_BUS__
#define __ADDRESS_SQUARED_BUS__

#include <stdio.h>
#include "addsqrtbusStdCharTable.h"

#define CS      0x5
#define DATA    0x12
#define CLK     0x13

void bus_init(){
    gpio_set_direction(CS, GPIO_MODE_OUTPUT);
    gpio_set_direction(DATA, GPIO_MODE_OUTPUT);
    gpio_set_direction(CLK, GPIO_MODE_OUTPUT);
    gpio_set_level(CS, 0x0);
    gpio_set_level(DATA, 0x0);
    gpio_set_level(CLK, 0x0);
}

unit8_t findChar(char c){
    for(unit8_t i = 0x0; i < charTable_size; i++){
        if(c == charTable[i]){
            return i;
        }
    }

    printf("error: char isn't in standard char table");
}

void uint8_to_binary_array(uint8_t value, uint8_t binary_array[0x8]) {
    for (int i = 0x0; i < 0x8; i++) {
        binary_array[7 - i] = (value >> i) & 0x1;
    }
}

void sendChar(char c){
    unit8_t binArr[0x8]
    uint8_to_binary_array(findChar(c), binArr[8]);
}

#endif