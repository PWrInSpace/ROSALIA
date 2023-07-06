idf_component_register(SRCS "lora_test_config.c"
                    INCLUDE_DIRS "."
                    REQUIRES cmock driver app_test_mode lora console fatfs app)

