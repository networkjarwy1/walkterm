#ifndef __WALKTERMLIB__
#define __WALKTERMLIB__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>

#include "freertos/projdefs.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_spiffs.h"
#include "esp_event.h"

#include "cardkb.h"
#include "fontx.h"

static const char *TAG = "walkterm";

extern uint8_t buffer[40];
extern QueueHandle_t keyQueue;

void traceHeap();
esp_err_t mountSPIFFS(char * path, char * label, int max_files);
void listSPIFFS(char * path);
void walkterm_init();
void font_init();
void KeyboardTask(void *pvParameters);
void DisplayTask(void *pvParameters);

#if CONFIG_WALKTERM_ST7789
#include "st7789.h"
#define X_ZERO (CONFIG_WIDTH-1)-16
#define DEFAULT_FONT fx16G

extern uint16_t text_color;
extern FontxFile fx16G[2];
extern FontxFile fx24G[2];
extern FontxFile fx32G[2];
extern FontxFile fx32L[2];
extern FontxFile fx16M[2];
extern FontxFile fx24M[2];
extern FontxFile fx32M[2];
#elif CONFIG_WALKTERM_SSD1306
// walkterm ssd1306 defines and variables
#endif

#endif // __WALKTERMLIB__
