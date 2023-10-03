// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#pragma once

#include "mcu_i2c_config.h"
#include "lora.h "
#include "lora_esp32_config.h"
#include "mcu_spi_config.h"
#include "ssd1306_esp32_config.h"
#include "mcu_uart_config.h"

typedef struct {
  mcu_spi_config_t spi;
  mcu_i2c_config_t i2c;
  lora_struct_t lora;
  ssd1306_t oled_display;
} ROSALIA_devices_t;
