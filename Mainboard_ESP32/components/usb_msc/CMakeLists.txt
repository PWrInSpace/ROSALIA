idf_component_register(SRC_DIRS "."
                    INCLUDE_DIRS "."
                    REQUIRES usb_msc driver usb vfs fatfs)

target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-format -Wall -W -Werror)