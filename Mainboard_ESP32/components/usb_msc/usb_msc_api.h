// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#pragma once

/*! \file usb_msc_api.h
 *  \brief USB MSC API
 *  This file contains API for USB MSC on ESP32S2 or ESP32S3.
 */

#include <dirent.h>
#include <errno.h>
#include <stdbool.h>

#include "esp_err.h"
#include "esp_log.h"
#include "tinyusb.h"
#include "tusb_msc_storage.h"

typedef enum {
  USB_MSC_INIT_STORAGE_TYPE_SDMMC = 0x01,
  USB_MSC_INIT_STORAGE_TYPE_SPI_FLASH = 0x10,
  USB_MSC_INIT_STORAGE_NONE = 0x00,
} usb_msc_init_storage_type_t;

/*!
 * \brief USB MSC configuration struct
 * \param desc_device USB device descriptor
 * \param desc_cfg USB configuration descriptor
 */
typedef struct {
  tusb_desc_device_t desc_device;
  tusb_desc_configuration_t desc_cfg;
  tinyusb_config_t tusb_cfg;
  tinyusb_msc_sdmmc_config_t sd_config;
  tinyusb_msc_spiflash_config_t spiflash_config;
  wl_handle_t wl_handle;
  const char* mount_path;
  usb_msc_init_storage_type_t current_storage_type;
} usb_msc_config_t;

/*!
 * \brief USB MSC device initialization
 * \param config USB MSC configuration
 */
esp_err_t usb_msc_init(usb_msc_config_t* config,
                       usb_msc_init_storage_type_t storage_type);

/*!
 * \brief USB MSC device deinitialization
 * \param config USB MSC configuration
 */
void usb_msc_deinit(usb_msc_config_t* config);

/*!
 * \brief Mount USB MSC storage
 * \param config USB MSC configuration
 */
esp_err_t usb_msc_mount(usb_msc_config_t* config);

/*!
 * \brief Unmount USB MSC storage
 * \param config USB MSC configuration
 */
esp_err_t usb_msc_unmount();

/*!
 * \brief Check if USB MSC storage is in use by host
 * \returns true if USB MSC storage is in use by host
 */
bool usb_msc_check_storage_use_by_host();

/*!
 * \brief Initialize USB MSC storage on SPI flash
 * \param config USB MSC configuration
 */
esp_err_t usb_msc_storage_init_spiflash(usb_msc_config_t* config);

/*!
 * \brief Deinitialize USB MSC storage on SPI flash
 * \param config USB MSC configuration
 */
esp_err_t usb_msc_storage_deinit_spiflash(usb_msc_config_t* config);
