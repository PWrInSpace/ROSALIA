// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#pragma once

#include "i2c_config.h"
#include "lora_esp32_config.h"
#include "spi_config.h"
#include "ssd1306_esp32_config.h"
#include "uart_config.h"

typedef struct {
  spi_config_t spi;
  i2c_config_t i2c;
  lora_struct_t lora;
  ssd1306_t oled_display;

} ROSALIA_devices_t;
