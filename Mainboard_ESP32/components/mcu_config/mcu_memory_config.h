// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#pragma once

#include "flash.h"
#include "flash_nvs.h"
#include "sdcard.h"
#include "usb_msc_api.h"

typedef struct {
  sd_card_config_t sd_card_config;
  sd_card_t sd_card;
  spi_bus_config_t spi_bus_config;
  usb_msc_config_t usb_msc_config;
} mcu_memory_config_t;

/*!
 * \brief Initialize memory
 * \param[in] memory_config memory config
 * \returns ESP_OK if successful, ESP_FAIL otherwise
 */
esp_err_t memory_init(mcu_memory_config_t* memory_config);

/*!
 * \brief Activates memory access through msc
 * \param[in] memory_config memory config
 * \param[in] storage_type storage type to be exposed - SD or FLASH
 */
esp_err_t memory_usb_msc_activate(mcu_memory_config_t* memory_config,
                                  usb_msc_init_storage_type_t storage_type);

/*!
 * \brief Deactivates memory access through msc
 * \param[in] memory_config memory config
 */
esp_err_t memory_usb_msc_deactivate(mcu_memory_config_t* memory_config);

/*!
 * \brief Mounts SPI config to memory config
 * \param[in] memory_config memory config
 * \param[in] spi_config spi config
 */
void memory_config_mount_spi_config(mcu_memory_config_t* memory_config,
                                    mcu_spi_config_t* mcu_spi_config);
