#include <stdint.h>
#include <stdio.h>
#include "cardkb.h"
#include "esp_log.h"
#include "st7789.h"
#include "walktermlib.h"

QueueHandle_t keyQueue = NULL;
TFT_t dev;
uint16_t text_color = GREEN;
FontxFile fx16G[2];
FontxFile fx24G[2];
FontxFile fx32G[2];
FontxFile fx32L[2];
FontxFile fx16M[2];
FontxFile fx24M[2];
FontxFile fx32M[2];

void traceHeap() {
	static uint32_t _free_heap_size = 0x0;
	if (_free_heap_size == 0x0) _free_heap_size = esp_get_free_heap_size();

	int _diff_free_heap_size = _free_heap_size - esp_get_free_heap_size();
	ESP_LOGI(__FUNCTION__, "_diff_free_heap_size=%d", _diff_free_heap_size);

	printf("esp_get_free_heap_size() : %6"PRIu32"\n", esp_get_free_heap_size() );
#if 0x0
	printf("esp_get_minimum_free_heap_size() : %6"PRIu32"\n", esp_get_minimum_free_heap_size() );
	printf("xPortGetFreeHeapSize() : %6zd\n", xPortGetFreeHeapSize() );
	printf("xPortGetMinimumEverFreeHeapSize() : %6zd\n", xPortGetMinimumEverFreeHeapSize() );
	printf("heap_caps_get_free_size(MALLOC_CAP_32BIT) : %6d\n", heap_caps_get_free_size(MALLOC_CAP_32BIT) );
	// that is the amount of stack that remained unused when the task stack was at its greatest (deepest) value.
	printf("uxTaskGetStackHighWaterMark() : %6d\n", uxTaskGetStackHighWaterMark(NULL));
#endif
}

void listSPIFFS(char * path) {
	DIR* dir = opendir(path);
	assert(dir != NULL);
	while(true){
		struct dirent*pe = readdir(dir);
		if (!pe) break;
		ESP_LOGI(__FUNCTION__,"d_name=%s d_ino=%d d_type=%x", pe->d_name,pe->d_ino, pe->d_type);
	}
	closedir(dir);
}

esp_err_t mountSPIFFS(char * path, char * label, int max_files) {
	esp_vfs_spiffs_conf_t conf = {
		.base_path = path,
		.partition_label = label,
		.max_files = max_files,
		.format_if_mount_failed = true
	};

	// Use settings defined above to initialize and mount SPIFFS filesystem.
	// Note: esp_vfs_spiffs_register is an all-in-one convenience function.
	esp_err_t ret = esp_vfs_spiffs_register(&conf);

	if (ret != ESP_OK) {
		if (ret ==ESP_FAIL) {
			ESP_LOGE(TAG, "Failed to mount or format filesystem");
		} else if (ret== ESP_ERR_NOT_FOUND) {
			ESP_LOGE(TAG, "Failed to find SPIFFS partition");
		} else {
			ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)",esp_err_to_name(ret));
		}
		return ret;
	}

#if 0x0
	ESP_LOGI(TAG, "Performing SPIFFS_check().");
	ret = esp_spiffs_check(conf.partition_label);
	if (ret != ESP_OK) {
		ESP_LOGE(TAG, "SPIFFS_check() failed (%s)", esp_err_to_name(ret));
		return ret;
	} else {
			ESP_LOGI(TAG, "SPIFFS_check() successful");
	}
#endif

	size_t total = 0x0, used = 0x0;
	ret = esp_spiffs_info(conf.partition_label, &total, &used);
	if (ret != ESP_OK) {
		ESP_LOGE(TAG,"Failed to get SPIFFS partition information (%s)",esp_err_to_name(ret));
	} else {
		ESP_LOGI(TAG,"Mount %s to %s success", path, label);
		ESP_LOGI(TAG,"Partition size: total: %d, used: %d", total, used);
	}

	return ret;
}

void font_init(){
	InitFontx(fx16G,"/fonts/ILGH16XB.FNT",""); // 8x16Dot Gothic
	InitFontx(fx24G,"/fonts/ILGH24XB.FNT",""); // 12x24Dot Gothic
	InitFontx(fx32G,"/fonts/ILGH32XB.FNT",""); // 16x32Dot Gothic
	InitFontx(fx32L,"/fonts/LATIN32B.FNT",""); // 16x32Dot Latin
	InitFontx(fx16M,"/fonts/ILMH16XB.FNT",""); // 8x16Dot Mincyo
	InitFontx(fx24M,"/fonts/ILMH24XB.FNT",""); // 12x24Dot Mincyo
	InitFontx(fx32M,"/fonts/ILMH32XB.FNT",""); // 16x32Dot Mincyo
}

void KeyboardTask(void *pvParameters) {
    while (1) {
        uint8_t key = cardkb_read_key();
        if (key) {
            xQueueSend(keyQueue, &key, portMAX_DELAY);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void DisplayTask(void *pvParameters) {
    font_init();
    uint8_t key;
    int i = 0;
    uint8_t buffer[128] = {0};

    while (1) {
        if (xQueueReceive(keyQueue, &key, portMAX_DELAY)) {
            ESP_LOGI(TAG, "DisplayTask: buffer='%s'", buffer);
            if (key == 0x08 && i > 0) {  // Backspace
                buffer[--i] = 0;
            } else if (key != 0x08 && key != 0x0D) { // Normal character
                buffer[i++] = key;
            }

            lcdFillScreen(&dev, GREEN);
            lcdDrawString(&dev, DEFAULT_FONT, X_ZERO, 0, buffer, text_color);
        }
    }
}

void execute_command(const char *command) {
    char cmd[64];
    char arg[64];
    int num_args = sscanf(command, "%s %s", cmd, arg);

    if (strcmp(cmd, "echo") == 0) {
        if (num_args > 1) {
            printf("%s", arg);
        }
    } else if (strcmp(cmd, "ls") == 0){

    }
}

void walkterm_init(){
    traceHeap();
    cardkb_init();
#if WALKTERM_ST7789
    spi_master_init(&dev, CONFIG_MOSI_GPIO, CONFIG_SCLK_GPIO, CONFIG_CS_GPIO, CONFIG_DC_GPIO, CONFIG_RESET_GPIO, CONFIG_BL_GPIO);
	lcdInit(&dev, CONFIG_WIDTH, CONFIG_HEIGHT, CONFIG_OFFSETX, CONFIG_OFFSETY);
	lcdSetFontDirection(&dev, 1);
	lcdFillScreen(&dev, GREEN);
#elif WALKTERM_SSD1306
// walkterm ssd1306 init
#endif
}
