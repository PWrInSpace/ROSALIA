// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#include "ble_api.h"

#define BLE_TEST_TAG "BLE_UNIT_TEST"

typedef enum {
  PROFILE_A,
  PROFILE_B,
  PROFILE_C,
  PROFILE_TOTAL_NUM
} ble_gap_profile_names_t;

static void gatts_event_handler(esp_gatts_cb_event_t event,
                                esp_gatt_if_t gatts_if,
                                esp_ble_gatts_cb_param_t* param);

static void gatt_profile_a_event_handler(esp_gatts_cb_event_t event,
                                         esp_gatt_if_t gatts_if,
                                         esp_ble_gatts_cb_param_t* param);

static void gatt_profile_b_event_handler(esp_gatts_cb_event_t event,
                                         esp_gatt_if_t gatts_if,
                                         esp_ble_gatts_cb_param_t* param);

static void gatt_profile_c_event_handler(esp_gatts_cb_event_t event,
                                         esp_gatt_if_t gatts_if,
                                         esp_ble_gatts_cb_param_t* param);

static uint8_t adv_config_done = 0;
#define adv_config_flag (1 << 0)
#define scan_rsp_config_flag (1 << 1)

static uint8_t adv_service_uuid128[32] = BLE_UUID_CONFIG_DEFAULT();

static esp_ble_adv_data_t adv_data = BLE_ADV_DATA_CONFIG_DEFAULT();

static esp_ble_adv_data_t scan_rsp_data = BLE_SCAN_RSP_DATA_CONFIG_DEFAULT();

static esp_ble_adv_params_t adv_params = BLE_ADV_PARAMS_CONFIG_DEFAULT();

ble_gatts_t ble_gatt_conf = {
    .profiles_num = PROFILE_TOTAL_NUM,
    .event_handler_cb = gatts_event_handler,
    .profiles = {[PROFILE_A] = {.gatts_cb = gatt_profile_a_event_handler,
                                .gatts_if = ESP_GATT_IF_NONE},
                 [PROFILE_B] = {.gatts_cb = gatt_profile_b_event_handler,
                                .gatts_if = ESP_GATT_IF_NONE},
                 [PROFILE_C] = {.gatts_cb = gatt_profile_c_event_handler,
                                .gatts_if = ESP_GATT_IF_NONE}}};

static void gatts_event_handler(esp_gatts_cb_event_t event,
                                esp_gatt_if_t gatts_if,
                                esp_ble_gatts_cb_param_t* param) {
  /* If event is register event, store the gatts_if for each profile */
  if (event == ESP_GATTS_REG_EVT) {
    if (param->reg.status == ESP_GATT_OK) {
      ble_gatt_conf.profiles[param->reg.app_id].gatts_if = gatts_if;
    } else {
      ESP_LOGI(BLE_TEST_TAG, "Reg app failed, app_id %04x, status %d\n",
               param->reg.app_id, param->reg.status);
      return;
    }
  }

  /* If the gatts_if equal to profile A, call profile A cb handler,
   * so here call each profile's callback */
  do {
    int idx;
    for (idx = 0; idx < PROFILE_TOTAL_NUM; idx++) {
      if (gatts_if == ESP_GATT_IF_NONE ||
          gatts_if == ble_gatt_conf.profiles[idx].gatts_if) {
        if (ble_gatt_conf.profiles[idx].gatts_cb) {
          ble_gatt_conf.profiles[idx].gatts_cb(event, gatts_if, param);
        }
      }
    }
  } while (0);
}
