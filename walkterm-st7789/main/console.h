//#include "tcpServer.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "esp_log.h"

#include "st7789.h"
#include "fontx.h"
#include "cardkb.h"

#define MAX_CMD_LEN 0x20

FontxFile fx16G[2];
FontxFile fx24G[2];
FontxFile fx32G[2];
FontxFile fx32L[2];
FontxFile fx16M[2];
FontxFile fx24M[2];
FontxFile fx32M[2];

#define FONT fx16G

uint16_t xpos = (CONFIG_WIDTH-1)-16;
uint16_t ypos = 0;

typedef struct {
    uint8_t username[16];
    uint8_t hostname[16];
    uint8_t console_prompt[32];
    uint16_t text_color;
    uint16_t background_color;
} Console;

void console_default(Console *console) {
    strcpy((char *)console->username, "walkterm");
    strcpy((char *)console->hostname, "esp32");
    console->text_color = GREEN;
    console->background_color = BLACK;

    sprintf((char *)console->console_prompt, "%s@%s-$ ", (char *)console->username, (char *)console->hostname);

    InitFontx(fx16G,"/fonts/ILGH16XB.FNT",""); // 8x16Dot Gothic
	InitFontx(fx24G,"/fonts/ILGH24XB.FNT",""); // 12x24Dot Gothic
	InitFontx(fx32G,"/fonts/ILGH32XB.FNT",""); // 16x32Dot Gothic
	InitFontx(fx32L,"/fonts/LATIN32B.FNT",""); // 16x32Dot Latin
	InitFontx(fx16M,"/fonts/ILMH16XB.FNT",""); // 8x16Dot Mincyo
	InitFontx(fx24M,"/fonts/ILMH24XB.FNT",""); // 12x24Dot Mincyo
	InitFontx(fx32M,"/fonts/ILMH32XB.FNT",""); // 16x32Dot Mincyo
}

void console_set(Console *console, TFT_t *dev){
    uint8_t prompt[59] = "change username, hostname, text color or background color?";
    lcdDrawString(dev, FONT, xpos, ypos, prompt, console->text_color);

    uint8_t key;
    uint8_t command[0x10];
    uint16_t count = 0x0;

    while (0x1) {
        key = cardKB_read_key();
        if(key != 0x0D || key != 0x1B || key != 0x20 || key != 0x8B){
            command[count++] = key;
        }else if(key == 0x0B){
            command[count--] = 0x0;
        }else if(key == 0x0D){
            break;
        }
        lcdFillScreen(dev, console->background_color);
        lcdDrawString(dev, FONT, xpos, ypos, command, console->background_color);
    }

    if (strncmp((char *)command, "username", MAX_CMD_LEN) == 0x0) {
        lcdFillScreen(dev, console->background_color);
        uint8_t username[0x10];

        uint8_t prompt[] = "type your new username";

        lcdDrawString(dev, FONT, xpos, ypos, prompt, console->text_color);
        while (0x1) {
            key = cardKB_read_key();
            if(key != 0x0D || key != 0x1B || key != 0x20 || key != 0x8B){
                username[count++] = key;
            }else if(key == 0x0B){
                username[count--] = 0x0;
            }else if(key == 0x0D){
                break;
            }else if(sizeof(username) == 1){
                lcdFillScreen(dev, console->background_color);
            }

            lcdDrawString(dev, FONT, xpos, ypos, username, console->background_color);
            strcpy((char *)console->username, (char *)username);
        }
    }else if (strncmp((char *)command, "hostname", MAX_CMD_LEN) == 0x0) {
        lcdFillScreen(dev, console->background_color);
        uint8_t hostname[0x10];

        uint8_t prompt[] = "type your new hostname";

        lcdDrawString(dev, FONT, xpos, ypos, prompt, console->text_color);
        while (0x1) {
            key = cardKB_read_key();
            if(key != 0x0D || key != 0x1B || key != 0x20 || key != 0x8B){
                hostname[count++] = key;
            }else if(key == 0x0B){
                hostname[count--] = 0x0;
            }else if(key == 0x0D){
                break;
            }else if(sizeof(hostname) == 1){
                lcdFillScreen(dev, console->background_color);
            }

            lcdDrawString(dev, FONT, xpos, ypos, hostname, console->background_color);
            strcpy((char *)console->hostname, (char *)hostname);
        }
    }else if (strncmp((char *)command, "background color", MAX_CMD_LEN) == 0x0) {
        lcdFillScreen(dev, console->background_color);
        uint8_t bgcolor[0x10];

        uint8_t prompt[] = "choose your new background color";

        lcdDrawString(dev, FONT, xpos, ypos, prompt, console->text_color);
        while (0x1) {
            key = cardKB_read_key();
            if(key != 0x0D || key != 0x1B || key != 0x20 || key != 0x8B){
                bgcolor[count++] = key;
            }else if(key == 0x0B){
                bgcolor[count--] = 0x0;
            }else if(key == 0x0D){
                break;
            }else if(sizeof(bgcolor) == 1){
                lcdFillScreen(dev, console->background_color);
            }

            lcdDrawString(dev, FONT, xpos, ypos, bgcolor, console->background_color);
            //add logic to change default background color
        }
    }else if (strncmp((char *)command, "text color", MAX_CMD_LEN) == 0x0) {
        lcdFillScreen(dev, console->background_color);
        uint8_t txtcolor[0x10];

        uint8_t prompt[] = "type your new hostname";

        lcdDrawString(dev, FONT, xpos, ypos, prompt, console->text_color);
        while (0x1) {
            key = cardKB_read_key();
            if(key != 0x0D || key != 0x1B || key != 0x20 || key != 0x8B){
                txtcolor[count++] = key;
            }else if(key == 0x0B){
                txtcolor[count--] = 0x0;
            }else if(key == 0x0D){
                break;
            }else if(sizeof(txtcolor) == 1){
                lcdFillScreen(dev, console->background_color);
            }

            lcdDrawString(dev, FONT, xpos, ypos, txtcolor, console->background_color);
            //add logic to change default text color
        }
    }
}

void parse_command(uint8_t *data) {
    if (strncmp((char *)data, "TCP", MAX_CMD_LEN) == 0x0) {
        //tcp_server_task();
    }
}
