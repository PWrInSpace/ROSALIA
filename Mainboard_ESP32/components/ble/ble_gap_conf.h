// Copyright 2023 PWr in Space, Krzysztof Gliwiński
#pragma once

#include "esp_gap_ble_api.h"

/*!
 * \file ble_gap_conf.h
 * \brief API for configurating the BLE GAP module
 */

typedef enum { GAP_OK = 0, GAP_ADV_ERR, GAP_ERR } ble_gap_err_t;

/*!
 * \brief GAP configuration structure
 * Used as a central device, only broadcaster, peripheral or both can be set
 */
typedef enum {
  BLE_GAP_BROADCASTER = 0,
  BLE_GAP_PERIPHERAL,
  BLE_GAP_BROADCASTER_PERIPHERAL
} ble_gap_conf_type_t;

typedef struct {
  ble_gap_conf_type_t conf_type;
  esp_ble_adv_params_t adv_params;
  esp_ble_adv_data_t adv_data;
  esp_ble_adv_data_t scan_rsp_data;
} ble_gap_t;

/*!
 * \brief Initialize the BLE GAP module
 * \param[in] gap_conf GAP configuration structure
 */
ble_gap_err_t ble_gap_init(ble_gap_t *gap_conf, ble_gap_conf_type_t type,
                           esp_ble_adv_params_t *adv_params);

/*!
 * \brief Start advertising
 */
ble_gap_err_t ble_gap_start_advertising(ble_gap_t *gap_conf);

// TODO(Glibus): Maybe will be obsolete
/*!
 * \brief Handle GAP events
 * \param[in] event GAP event
 * \param[in] param GAP event parameters
 */
void ble_gap_event_handler(esp_gap_ble_cb_event_t event,
                           esp_ble_gap_cb_param_t *param);
