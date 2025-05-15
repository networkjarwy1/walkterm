# driver for M5stack cardkb I2C keyboard
## usage
### reading key
```c
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "cardkb.h"

void app_main() {
    cardkb_init();

    while (1) {
        uint8_t key = cardkb_read_key();
        if (key) {
            // handle returned key here
        }
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}
```

### reading word
```c
#include <stdint.h>
#include "cardkb.h"

#define WORD_SIZE 64

void app_main(){
    uint8_t word[WORD_SIZE]

    cardkb_init();

    cardkb_read_word(&word, WORD_SIZE);

    // handle returned word here
}
```

### you need to wire the cardkb to I2C bus on esp32, then using menuconfig configure how its wired and you can choose I2C frequency
