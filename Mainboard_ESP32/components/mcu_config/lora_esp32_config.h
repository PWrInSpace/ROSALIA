// Copyright 2023 PWr in Space, Krzysztof Gliwiński
#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_log.h"
#include "esp_rom_gpio.h"
#include "freertos/task.h"
#include "mcu_spi_config.h"
#include "rom/gpio.h"
#include "sdkconfig.h"
#include "soc/gpio_struct.h"

/*!
 * \file lora_esp32_config.h
 * \brief ESP32 lora configuration
 */

void lora_esp32_config_mount_spi_config(mcu_spi_config_t* _spi_config);

esp_err_t _lora_spi_and_pins_init(gpio_num_t _rst_gpio_num,
                                  gpio_num_t _cs_gpio_num,
                                  gpio_num_t _d0_gpio_num);

bool _lora_SPI_transmit(uint8_t _in[2], uint8_t _out[2]);

void _lora_delay(size_t _ms);

bool _lora_GPIO_set_level(uint8_t _gpio_num, uint32_t _level);

void _lora_log(const char* info);
