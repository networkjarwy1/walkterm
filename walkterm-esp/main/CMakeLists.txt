idf_component_register(SRCS "walkterm-esp.c" "addsqrtbus.h" "addsqrtbusStdCharTable.h"
                    INCLUDE_DIRS "."
                    REQUIRES esp_driver_gpio
                    REQUIRES driver)
