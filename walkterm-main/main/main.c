#include "walktermlib.h"

void app_main(void){
	ESP_LOGI(TAG, "Initializing SPIFFS");
	// Maximum files that could be open at the same time is 7.
	ESP_ERROR_CHECK(mountSPIFFS("/fonts", "storage1", 0x7));
	listSPIFFS("/fonts/");
    //ESP_ERROR_CHECK(esp_event_loop_create_default());
    walkterm_init();

    // Create the queue with size and item size
    keyQueue = xQueueCreate(32, sizeof(uint8_t));
    if (keyQueue == NULL) {
        ESP_LOGE(TAG, "Failed to create keyQueue!");
        return;
    }

    xTaskCreatePinnedToCore(
            KeyboardTask, "KeyboardTask",
            2048, NULL, 5, NULL, 0
        );

        // Create DisplayTask on core 1
        xTaskCreatePinnedToCore(
            DisplayTask, "DisplayTask",
            4096, NULL, 5, NULL, 1
        );
}
