idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS ""
                    REQUIRES driver lora ble misc ssd1306 console fatfs mcu_config app flash_memory sd_card usb_msc)

target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-format -Wall -W -Werror)
