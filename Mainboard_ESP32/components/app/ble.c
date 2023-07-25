// Copyright 2023 PWr in Space, Krzysztof Gliwiński
#include "ble.h"

#define BLE_APP_TAG "BLE_APP"

typedef enum {
  PROFILE_A,
  PROFILE_B,
  PROFILE_TOTAL_NUM
} ble_gap_profile_names_t;

static uint8_t adv_service_uuid128[32] = {
    /* LSB <--------------------------------------------------------------------------------> MSB */
    //first uuid, 16bit, [12],[13] is the value
    0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0xEE, 0x00, 0x00, 0x00,
    //second uuid, 32bit, [12], [13], [14], [15] is the value
    0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00,
};

static esp_ble_adv_data_t adv_data = {
    .set_scan_rsp = false,
    .include_name = true,
    .include_txpower = false,
    .min_interval = 0x0006, //slave connection min interval, Time = min_interval * 1.25 msec
    .max_interval = 0x0010, //slave connection max interval, Time = max_interval * 1.25 msec
    .appearance = 0x00,
    .manufacturer_len = 0, //TEST_MANUFACTURER_DATA_LEN,
    .p_manufacturer_data =  NULL, //&test_manufacturer[0],
    .service_data_len = 0,
    .p_service_data = NULL,
    .service_uuid_len = sizeof(adv_service_uuid128),
    .p_service_uuid = adv_service_uuid128,
    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};
// scan response data
static esp_ble_adv_data_t scan_rsp_data = {
    .set_scan_rsp = true,
    .include_name = true,
    .include_txpower = true,
    //.min_interval = 0x0006,
    //.max_interval = 0x0010,
    .appearance = 0x00,
    .manufacturer_len = 0, //TEST_MANUFACTURER_DATA_LEN,
    .p_manufacturer_data =  NULL, //&test_manufacturer[0],
    .service_data_len = 0,
    .p_service_data = NULL,
    .service_uuid_len = sizeof(adv_service_uuid128),
    .p_service_uuid = adv_service_uuid128,
    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};

static esp_ble_adv_params_t adv_params = {
    .adv_int_min        = 0x20,
    .adv_int_max        = 0x40,
    .adv_type           = ADV_TYPE_IND,
    .own_addr_type      = BLE_ADDR_TYPE_PUBLIC,
    //.peer_addr            =
    //.peer_addr_type       =
    .channel_map        = ADV_CHNL_ALL,
    .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

static ble_gap_t ble_gap_conf = {
    .conf_type = BLE_GAP_BROADCASTER_CENTRAL,
    .adv_params = adv_params,
    .adv_data = adv_data,
    .scan_rsp_data = scan_rsp_data,
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
//   vTaskDelete(NULL);
}
