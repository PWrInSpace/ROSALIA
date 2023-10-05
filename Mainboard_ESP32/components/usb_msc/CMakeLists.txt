idf_component_register(SRC_DIRS "."
                    INCLUDE_DIRS "."
                    REQUIRES driver usb vfs fatfs espressif__esp_tinyusb)

target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-format -Wall -W -Werror)