// Copyright 2023 PWr in Space, Krzysztof Gliwiński
#include "ble.h"

#define BLE_APP_TAG "BLE_APP"

typedef enum {
  PROFILE_A,
  PROFILE_B,
  PROFILE_TOTAL_NUM
} ble_gap_profile_names_t;

static ble_gap_t ble_gap_conf = {
    .conf_type = BLE_GAP_BROADCASTER_CENTRAL,
    .adv_params =
        {
            .adv_int_min = 0x20,
            .adv_int_max = 0x40,
            .adv_type = ADV_TYPE_IND,
            .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
            .channel_map = ADV_CHNL_ALL,
            .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
        },
    .adv_data =
        {
            .set_scan_rsp = false,
            .include_name = true,
            .include_txpower = true,
            .min_interval = 0x20,
            .max_interval = 0x40,
            .appearance = 0x00,
            .manufacturer_len = 0,
            .p_manufacturer_data = NULL,
            .service_data_len = 0,
            .p_service_data = NULL,
            .service_uuid_len = 0,
            .p_service_uuid = NULL,
            .flag = 0,
        },
    .scan_rsp_data =
        {
            .set_scan_rsp = true,
            .include_name = true,
            .include_txpower = true,
            .min_interval = 0x20,
            .max_interval = 0x40,
            .appearance = 0x00,
            .manufacturer_len = 0,
            .p_manufacturer_data = NULL,
            .service_data_len = 0,
            .p_service_data = NULL,
            .service_uuid_len = 0,
            .p_service_uuid = NULL,
            .flag = 0,
        },
    .event_handler_cb = gap_event_handler};

static ble_gatts_t ble_gatt_conf = {.profiles_num = PROFILE_TOTAL_NUM,
                                    .event_handler_cb = gatt_event_handler};

static ble_config_t ble_conf = {.gap_config = &ble_gap_conf,
                                .gatt_config = &ble_gatt_conf};

void gap_event_handler(esp_gap_ble_cb_event_t event,
                       esp_ble_gap_cb_param_t* param) {
  switch (event) {
    case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
      esp_ble_gap_start_advertising(&(ble_gap_conf.adv_params));
      break;
    default:
      break;
  }
}

void gatt_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if,
                        esp_ble_gatts_cb_param_t* param) {
  switch (event) {
    case ESP_GATTS_REG_EVT:
      esp_ble_gap_set_device_name(BLE_DEVICE_NAME);
      esp_ble_gap_config_adv_data(&(ble_gap_conf.adv_data));
      esp_ble_gap_config_adv_data(&(ble_gap_conf.scan_rsp_data));
      break;
    default:
      break;
  }
}

void ble_init_task(void* arg) {
  ble_err_t ret = ble_init(&ble_conf);
  if (ret != BLE_OK) {
    ESP_LOGE(BLE_APP_TAG, "BLE init failed: %s", __func__);
  }
  vTaskDelete(NULL);
}
