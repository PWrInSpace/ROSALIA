// Copyright 2023 PWr in Space, Krzysztof Gliwiński
#pragma once

#include <stdint.h>

#include "ble_gap_conf.h"
#include "ble_gatt_conf.h"

/*!
 * \file ble_api.h
 * \brief BLE API - contains all BLE functions and structures
 * ass well as attributes of GATT - unified in one structure
 */

/*!
 * \brief BLE status enum
 */
typedef enum { BLE_OK = 0, BLE_ERR } ble_err_t;

/*!
 * \brief Main BLE configuration struct.
 * Contains //TODO(Glibus): dopisac co i jak
 */
typedef struct {
  ble_gatts_t gatts_config;
  ble_gap_t gap_config;
} ble_config_t;
