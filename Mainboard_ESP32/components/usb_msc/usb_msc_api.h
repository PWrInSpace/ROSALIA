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
  const char* mount_path;
} usb_msc_config_t;

/*!
 * \brief USB MSC device initialization
 * \param config USB MSC configuration
 */
esp_err_t usb_msc_init(usb_msc_config_t* config);

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
