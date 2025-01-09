#include "cardkb.h"
#include "addsqrtbus.h"

void app_main() {
    cardKB_init();
    bus_init();

    while (1) {
        char key = cardkb_read_key();
        if (key) {
            sendChar(key);
        }
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}
