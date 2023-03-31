idf_component_register(SRCS "ble_api.c"
                    INCLUDE_DIRS "."
                    REQUIRES cmock driver soc esp_rom ble)

