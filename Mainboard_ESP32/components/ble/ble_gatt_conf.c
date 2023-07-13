// Copyright 2023 PWr in Space, Krzysztof Gliwiński
#include "ble_gatt_conf.h"

#define BLE_GATT_TAG "BLE_GATT_API"

esp_err_t ble_gatt_init(ble_gatts_t *gatts_conf) {
  esp_err_t ret = ESP_OK;

  for (uint16_t i = 0; i < gatts_conf->gatts_profiles_num; ++i) {
    ret = esp_ble_gatts_app_register(gatts_conf->gatts_profiles[i].app_id);
    if (ret != ESP_OK) {
      ESP_LOGE(BLE_GATT_TAG, "Failed to register app: %s, %s", __func__,
               esp_err_to_name(ret));
      return ret;
    }
  }

  return ret;
}
