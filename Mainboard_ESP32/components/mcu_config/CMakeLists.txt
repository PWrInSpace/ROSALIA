idf_component_register(SRCS "lora_esp32_config.c" "mcu_memory_config.c" "mcu_misc_config.c" "mcu_i2c_config.c" "mcu_spi_config.c" "mcu_adc_config.c" "mcu_twai_config.c" "ssd1306_esp32_config.c"
                    INCLUDE_DIRS "."
                    REQUIRES cmock driver soc esp_rom lora ssd1306 esp_adc freertos flash_memory sd_card usb_msc misc)

target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-format -Wall -W -Werror)