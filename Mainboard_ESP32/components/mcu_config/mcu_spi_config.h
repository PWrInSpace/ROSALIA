// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#pragma once

#include <stdbool.h>

#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_log.h"
#include "esp_rom_gpio.h"
#include "freertos/task.h"
#include "rom/gpio.h"
#include "sdkconfig.h"
#include "soc/gpio_struct.h"

typedef struct {\
  spi_host_device_t host_id;
  spi_device_handle_t spi_handle;
  spi_bus_config_t bus_config;
  spi_device_interface_config_t dev_config;
  bool spi_init_flag;
} mcu_spi_config_t;

/*!
 * \brief Initiates the SPI bus
 * \param spi SPI configuration
 * \return ESP_OK on success, ESP_FAIL otherwise
 * \note This function will only initiate SPI peripheral,
 *      it will not configure the GPIO for CS pins.
 */
esp_err_t spi_init(mcu_spi_config_t *spi_config);
