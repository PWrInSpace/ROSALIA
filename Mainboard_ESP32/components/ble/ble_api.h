// Copyright 2023 PWr in Space, Krzysztof Gliwiński
#pragma once

#include <stdint.h>

#include "ble_gap_conf.h"
#include "ble_gatt_conf.h"
#include "esp_bt.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "sdkconfig.h"
#include "nvs_flash.h"

/*!
 * \file ble_api.h
 * \brief BLE API - contains all BLE functions and structures
 * ass well as attributes of GATT - unified in one structure
 */

/*!
 * \brief BLE status enum
 */
typedef enum { BLE_OK = 0, BLE_HARDWARE_INIT_ERR, BLE_ERR } ble_err_t;

/*!
 * \brief Main BLE configuration struct.
 * Contains gatts and gap configuration
 */
typedef struct {
  ble_gatts_t *gatt_config;
  ble_gap_t *gap_config;
  esp_bt_controller_config_t bt_config;
} ble_config_t;

/*!
 * \brief Initiates all necessary esp hardware related to bluetooth
 */
ble_err_t ble_esp_hardware_init(ble_config_t *ble);

/*!
 * \brief Initiates the whole bluetooth stack, as well as gap ble_gatts_t and
 * ble_gap_t configuration
 */
ble_err_t ble_init(ble_config_t *ble);
