// Copyright 2023 PWr in Space, Krzysztof Gliwiński
#include "ble_gap_conf.h"

#define GAP_TAG "BLE_GAP"

bool ble_gap_init(ble_gap_t *gap_conf, ble_gap_conf_type_t type,
                  esp_ble_adv_params_t *adv_params) {
  esp_err_t ret = ESP_OK;
  gap_conf->conf_type = type;
  gap_conf->adv_params = *adv_params;
  return ret;
}

bool ble_gap_start_advertising(ble_gap_t *gap_conf) {
  esp_err_t ret = ESP_OK;

  return ret;
}
