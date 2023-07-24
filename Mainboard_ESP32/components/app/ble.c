// Copyright 2023 PWr in Space, Krzysztof Gliwiński
#include "ble.h"

static ble_gap_t = {.conf_type = BLE_GAP_BROADCASTER_CENTRAL,
                    .adv_params = {
                            .adv_int_min = 0x20,
                            .adv_int_max = 0x40,
                            .adv_type = ADV_TYPE_IND,
                            .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
                            .channel_map = ADV_CHNL_ALL,
                            .adv_filter_policy =
                                ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
                        },
                    .adv_data = {
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
                            .p_slave_conn_int = NULL,
                            .slave_conn_int_len = 0,
                            .p_tx_power = NULL,
                        },
                    .scan_rsp_data = {
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
                            .p_slave_conn_int = NULL,
                            .slave_conn_int_len = 0,
                            .p_tx_power = NULL,
                        },
                    .event_handler = gap_event_handler}

static ble_gatts_t = {
    .profiles_num = PROFILE_TOTAL_NUM,
    .profiles = {
        {.app_id = PROFILE_A, .event_handler_cb = ble_a_event_handler},
        {.app_id = PROFILE_B, .event_handler_cb = ble_b_event_handler}}};

void gap_event_handler(esp_gap_ble_cb_event_t event,
                       esp_ble_gap_cb_param_t* param) {
  switch (event) {
    case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
      esp_ble_gap_start_advertising(&ble_gap_t.adv_params);
      break;
    default:
      break;
  }
}

void ble_init_task(void* arg) { return; }
