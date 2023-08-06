idf_component_register(SRCS "ble.c"
                    INCLUDE_DIRS "."
                    REQUIRES cmock driver app_test_mode lora console fatfs app ble)

