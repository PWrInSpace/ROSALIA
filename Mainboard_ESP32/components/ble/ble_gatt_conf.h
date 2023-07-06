// Copyright 2023 PWr in Space, Krzysztof Gliwiński
#pragma once

#include "esp_gatts_api.h"

/*!
 * \file ble_gatt_conf.h
 * \brief BLE GATT configuration structure
 */

/*!
 * \brief Handle GATT events
 * \param[in] event GATT event
 * \param[in] param GATT event parameters
 */
typedef void (*ble_gatt_event_handler)(esp_gatts_cb_event_t event,
                                       esp_gatt_if_t gatts_if,
                                       esp_ble_gatts_cb_param_t *param);

/*!
 * \brief BLE GATT configuration structure
 */
typedef struct {
  //   esp_gatts_attr_db_t *gatt_db;
  esp_gatts_cb_t gatts_cb;
  uint16_t gatts_if;
  uint16_t app_id;
  uint16_t conn_id;
  uint16_t service_handle;
  esp_gatt_srvc_id_t service_id;
  uint16_t char_handle;
  esp_bt_uuid_t char_uuid;
  esp_gatt_perm_t perm;
  esp_gatt_char_prop_t property;
  uint16_t descr_handle;
  esp_bt_uuid_t descr_uuid;
} ble_gatts_profiles_t;

/*!
 * \brief Main gatts configuration structure
 */
typedef struct {
  ble_gatt_event_handler *event_handler;
  uint16_t gatts_profiles_num;
  ble_gatts_profiles_t *gatts_profiles;
} ble_gatts_t;

/*!
 * \brief Initialize the BLE GATT module
 * \param[in] gatts_conf GATT configuration structure
 */
bool ble_gatt_init(ble_gatts_t *gatts_conf);

/*!
 * \brief Register the BLE GATT module
 * \param[in] gatts_conf GATT configuration structure
 */
bool ble_gatt_register(ble_gatts_t *gatts_conf);
