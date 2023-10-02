// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#include "ble_api.h"

#define BLE_TAG "BLE_API"

ble_err_t ble_esp_hardware_init(ble_config_t *ble) {
  esp_err_t ret = ESP_OK;
  // Initialize NVS.
  ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));
  esp_bt_controller_config_t bt_cfg =
      BT_CONTROLLER_INIT_CONFIG_DEFAULT();  // TODO(Glibus): check if this can
                                            // be passed in struct declaration

  ret = esp_bt_controller_init(&bt_cfg);
  if (ret != ESP_OK) {
    ESP_LOGE(BLE_TAG, "%s initialize controller failed: %s\n", __func__,
             esp_err_to_name(ret));
    return BLE_HARDWARE_INIT_ERR;
  }

  ble->bt_cfg = bt_cfg;

  ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
  if (ret != ESP_OK) {
    ESP_LOGE(BLE_TAG, "%s enable controller failed: %s\n", __func__,
             esp_err_to_name(ret));
    return BLE_HARDWARE_INIT_ERR;
  }
  ret = esp_bluedroid_init();
  if (ret != ESP_OK) {
    ESP_LOGE(BLE_TAG, "%s init bluetooth failed: %s\n", __func__,
             esp_err_to_name(ret));
    return BLE_HARDWARE_INIT_ERR;
  }
  ret = esp_bluedroid_enable();
  if (ret != ESP_OK) {
    ESP_LOGE(BLE_TAG, "%s enable bluetooth failed: %s\n", __func__,
             esp_err_to_name(ret));
    return BLE_HARDWARE_INIT_ERR;
  }

  return BLE_OK;
}

ble_err_t ble_init(ble_config_t *ble) {
  ble_err_t ret = ble_esp_hardware_init(ble);
  if (ret != BLE_OK) {
    return ret;
  }

  ret = ble_gap_init(ble->gap_config);
  if (ret != BLE_OK) {
    return ret;
  }

  ret = ble_gatt_init(ble->gatt_config);
  if (ret != BLE_OK) {
    return ret;
  }

  return BLE_OK;
}
