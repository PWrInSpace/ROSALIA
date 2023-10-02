// Copyright 2023 PWr in Space, Krzysztof Gliwiński
#include "ble.h"

#define BLE_APP_TAG "BLE_APP"

typedef enum { PROFILE_A, PROFILE_TOTAL_NUM } ble_gap_profile_names_t;

static uint8_t adv_config_done = 0;
#define adv_config_flag (1 << 0)
#define scan_rsp_config_flag (1 << 1)

static uint8_t adv_service_uuid128[32] = {
    /* LSB
       <-------------------------------------------------------------------------------->
       MSB */
    // first uuid, 16bit, [12],[13] is the value
    0xfb,
    0x34,
    0x9b,
    0x5f,
    0x80,
    0x00,
    0x00,
    0x80,
    0x00,
    0x10,
    0x00,
    0x00,
    0xEE,
    0x00,
    0x00,
    0x00,
    // second uuid, 32bit, [12], [13], [14], [15] is the value
    0xfb,
    0x34,
    0x9b,
    0x5f,
    0x80,
    0x00,
    0x00,
    0x80,
    0x00,
    0x10,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x00,
    0x00,
};

static esp_ble_adv_data_t adv_data = {
    .set_scan_rsp = false,
    .include_name = true,
    .include_txpower = false,
    .min_interval = 0x0006,  // slave connection min interval, Time =
                             // min_interval * 1.25 msec
    .max_interval = 0x0010,  // slave connection max interval, Time =
                             // max_interval * 1.25 msec
    .appearance = 0x00,
    .manufacturer_len = 0,        // TEST_MANUFACTURER_DATA_LEN,
    .p_manufacturer_data = NULL,  // &test_manufacturer[0],
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
    // .min_interval = 0x0006,
    // .max_interval = 0x0010,
    .appearance = 0x00,
    .manufacturer_len = 0,        // TEST_MANUFACTURER_DATA_LEN,
    .p_manufacturer_data = NULL,  // &test_manufacturer[0],
    .service_data_len = 0,
    .p_service_data = NULL,
    .service_uuid_len = sizeof(adv_service_uuid128),
    .p_service_uuid = adv_service_uuid128,
    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};

static esp_ble_adv_params_t adv_params = {
    .adv_int_min = 0x20,
    .adv_int_max = 0x40,
    .adv_type = ADV_TYPE_IND,
    .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
    .channel_map = ADV_CHNL_ALL,
    .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

static ble_gap_t ble_gap_conf = {.conf_type = BLE_GAP_BROADCASTER_CENTRAL,
                                 .event_handler_cb = gap_event_handler};

// static ble_gatts_profile_t rosalia_gatt_profiles[PROFILE_TOTAL_NUM] = {
// {.gatts_cb = gatt_profile_a_event_handler, .gatts_if = ESP_GATT_IF_NONE}};

static ble_gatts_t ble_gatt_conf = {
    .profiles_num = PROFILE_TOTAL_NUM,
    .event_handler_cb = gatts_event_handler,
    .profiles = {[PROFILE_A] = {.gatts_cb = gatt_profile_a_event_handler,
                                .gatts_if = ESP_GATT_IF_NONE}}};

static ble_config_t ble_conf = {
    .gap_config = &ble_gap_conf,
    .gatt_config = &ble_gatt_conf,
};

void gap_event_handler(esp_gap_ble_cb_event_t event,
                       esp_ble_gap_cb_param_t* param) {
  switch (event) {
    case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
      esp_err_t ret = esp_ble_gap_start_advertising(&(ble_gap_conf.adv_params));
      if (ret != ESP_OK) {
        ESP_LOGE(BLE_APP_TAG, "Advertising start failed %s", __func__);
      }
      break;
    default:
      break;
  }
}

void gatt_profile_a_event_handler(esp_gatts_cb_event_t event,
                                  esp_gatt_if_t gatts_if,
                                  esp_ble_gatts_cb_param_t* param) {
  switch (event) {
    case ESP_GATTS_REG_EVT:
      esp_err_t set_dev_name_ret = esp_ble_gap_set_device_name(BLE_DEVICE_NAME);
      if (set_dev_name_ret == ESP_FAIL) {
        ESP_LOGE(BLE_APP_TAG, "Set BLE device name failed, error code = %x",
                 set_dev_name_ret);
      } else {
        ESP_LOGI(BLE_APP_TAG, "Setting device name success");
      }
      esp_err_t ret = esp_ble_gap_config_adv_data(&adv_data);
      if (ret) {
        ESP_LOGE(BLE_APP_TAG, "config adv data failed, error code = %x", ret);
      }
      adv_config_done |= adv_config_flag;
      ret = esp_ble_gap_config_adv_data(&scan_rsp_data);
      if (ret) {
        ESP_LOGE(BLE_APP_TAG,
                 "config scan response data failed, error code = %x", ret);
      }
      adv_config_done |= scan_rsp_config_flag;
      esp_ble_gatts_create_service(
          gatts_if, &ble_gatt_conf.profiles[PROFILE_A].service_id, 4);

      break;
    default:
      break;
  }
}

void gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if,
                         esp_ble_gatts_cb_param_t* param) {
  /* If event is register event, store the gatts_if for each profile */
  if (event == ESP_GATTS_REG_EVT) {
    if (param->reg.status == ESP_GATT_OK) {
      ble_gatt_conf.profiles[param->reg.app_id].gatts_if = gatts_if;
    } else {
      ESP_LOGI(BLE_APP_TAG, "Reg app failed, app_id %04x, status %d\n",
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

bool ble_config_init(void) {
  ble_gap_conf.adv_params = adv_params;
  ble_gap_conf.adv_data = adv_data;
  ble_gap_conf.scan_rsp_data = scan_rsp_data;

  // if (sizeof(ble_gatt_conf.profiles) / sizeof(ble_gatts_profile_t) !=
  //     PROFILE_TOTAL_NUM) {
  //   ESP_LOGE(BLE_APP_TAG, "Profile number is not correct");
  //   return false;
  // }

  // for (uint8_t i = 0; i < ble_gatt_conf.profiles_num; i++) {
  //   ble_gatt_conf.profiles[i] = ble_gatt_conf.profiles[i];
  // }

  return true;
}

void ble_init_task(void* arg) {
  if (ble_config_init() != true) {
    ESP_LOGE(BLE_APP_TAG, "BLE config init failed: %s", __func__);
    return;
  }

  if (ble_init(&ble_conf) != BLE_OK) {
    ESP_LOGE(BLE_APP_TAG, "BLE init failed: %s", __func__);
    return;
  }

  ESP_LOGI(BLE_APP_TAG, "BLE init done");

  // ESP_LOGI(BLE_APP_TAG, "%d", ble_conf.bt_cfg);
  //   vTaskDelete(NULL);
}
