idf_component_register(SRCS "ble.c" "user_interface.c"
                    INCLUDE_DIRS "."
                    REQUIRES cmock driver app_test_mode lora console fatfs app ble misc mcu_config ssd1306)

target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-format -Wall -W -Werror)