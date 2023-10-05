// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#pragma once

/*! \file usb_msc_api.h
 *  \brief USB MSC API
 *  This file contains API for USB MSC on ESP32S2 or ESP32S3.
 */

#include <dirent.h>
#include <errno.h>

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
} usb_msc_config_t;

/*!
 * \brief USB MSC device initialization
 * \param config USB MSC configuration
 */
esp_err_t usb_msc_init(usb_msc_config_t* config);
