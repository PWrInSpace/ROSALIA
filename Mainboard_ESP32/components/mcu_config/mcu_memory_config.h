// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#pragma once

#include "flash_api.h"
#include "flash_nvs.h"
#include "mcu_spi_config.h"
#include "sdcard.h"
#include "sdkconfig.h"
#include "usb_msc_api.h"

#define SD_CARD_DEFAULT_CONFIG(sdmmc_card)                     \
  {                                                            \
    .card = &sdmmc_card, .spi_host = SPI2_HOST,                \
    .mount_point = SDCARD_MOUNT_POINT, .cs_pin = CONFIG_SD_CS, \
    .card_detect_pin = CONFIG_SD_CD, .mounted = false,         \
  }

#define SD_CARD_CONFIG_DEFAULT_CONFIG()                       \
  {                                                           \
    .spi_host = SPI2_HOST, .mount_point = SDCARD_MOUNT_POINT, \
    .cs_pin = CONFIG_SD_CS, .cd_pin = CONFIG_SD_CD,           \
  }

#define USB_MSC_CONFIG_DEFAULT_CONFIG(descriptor_config, string_desc_arr,      \
                                      desc_configuration, config_sdmmc,        \
                                      config_spi)                              \
  {                                                                            \
    .desc_device = descriptor_config,                                          \
    .tusb_cfg =                                                                \
        {                                                                      \
            .device_descriptor = &descriptor_config,                           \
            .string_descriptor = string_desc_arr,                              \
            .string_descriptor_count =                                         \
                sizeof(string_desc_arr) / sizeof(string_desc_arr[0]),          \
            .external_phy = false,                                             \
            .configuration_descriptor = desc_configuration,                    \
        },                                                                     \
    .sd_config = config_sdmmc, .spiflash_config = config_spi,                  \
    .mount_path = BASE_PATH, .current_storage_type = USB_MSC_INIT_STORAGE_NONE \
  }

typedef struct {
  sd_card_config_t* sd_card_config;
  sd_card_t* sd_card;
  usb_msc_config_t* usb_msc_config;
} mcu_memory_config_t;

/*!
 * \brief Initialize memory
 * \param[in] memory_config memory config
 * \returns ESP_OK if successful, ESP_FAIL otherwise
 */
esp_err_t memory_init(mcu_memory_config_t* memory_config);

/*!
 * \brief Deinitialize memory
 * \param[in] memory_config memory config
 * \returns ESP_OK if successful, ESP_FAIL otherwise
 */
esp_err_t memory_deinit(mcu_memory_config_t* memory_config);

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
void memory_usb_msc_deactivate(mcu_memory_config_t* memory_config);

/*!
 * \brief Mounts SPI config to memory config
 * \param[in] memory_config memory config
 * \param[in] spi_config spi config
 */
void memory_config_mount_spi_config(mcu_memory_config_t* memory_config,
                                    mcu_spi_config_t* mcu_spi_config);
