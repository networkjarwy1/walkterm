#include "cardkb.h"
#include "NSI.h"

void app_main() {
    cardKB_init();
    NSI_bus_init();

    while (1) {
        char key = cardkb_read_key();
        if (key) {
        }
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}
