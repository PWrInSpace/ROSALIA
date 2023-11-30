// Copyright 2023 PWr in Space, Krzysztof Gliwiński
#include "ble.h"

#define BLE_APP_TAG "BLE_APP"

#define adv_config_flag (1 << 0)
#define scan_rsp_config_flag (1 << 1)

typedef enum {
  MAINBOARD_SERVICE,
  MAIN_VALVE_SERVICE,
  LOADCELL_A,
  LOADCELL_B,
  FILLING_STATION,
  PROFILE_TOTAL_NUM
} rosalia_ble_gatt_profile_names_t;

typedef enum {
  MAINBOARD_SERVICE_IDX,
  MAINBOARD_SERVICE_VAL_IDX,
  VBAT_MEASUREMENTS_IDX,
  VBAT_MEASUREMENTS_VAL_IDX,
  ROSALIA_STATE_IDX,
  ROSALIA_STATE_VAL_IDX,
  AVAILABLE_STATES_IDX,
  AVAILABLE_STATES_VAL_IDX,
  COMMAND_IDX,
  COMMAND_VAL_IDX,
  COMMAND_CFG_IDX,
  MAINBOARD_MAX_IDX
} rosalia_gatt_db_mainboard_service_idxs;

typedef enum {
  MAINBOARD_SERVICE_UUID = 0x0001,
  VBAT_MEASUREMENTS_UUID = 0x0002,
  ROSALIA_STATE_UUID = 0x0003,
  AVAILABLE_STATES_UUID = 0x0004,
  COMMAND_UUID = 0x0005,
} rosalia_mainboard_service_uuids;

typedef enum {
  MAIN_VALVE_BATTERY_VOLTAGE_IDX,
  MAIN_VALVE_BATTERY_VOLTAGE_VAL_IDX,
  MAIN_VALVE_THERMOCOUPLES_IDX,
  MAIN_VALVE_THERMOCOUPLES_VAL_IDX,
  MAIN_VALVE_VALVE_STATE_IDX,
  MAIN_VALVE_VALVE_STATE_VAL_IDX,
  MAIN_VALVE_WAKEN_UP_IDX,
  MAIN_VALVE_WAKEN_UP_VAL_IDX,
  MAIN_VALVE_MAX_IDX
} rosalia_main_valve_service_idxs;

typedef enum {
  VENT_VALVE_BATTERY_VOLTAGE_IDX,
  VENT_VALVE_BATTERY_VOLTAGE_VAL_IDX,
  VENT_VALVE_PRESSURE_IDX,
  VENT_VALVE_PRESSURE_VAL_IDX,
  VENT_VALVE_THERMISTORS_IDX,
  VENT_VALVE_THERMISTORS_VAL_IDX,
  VENT_VALVE_VALVE_STATE_IDX,
  VENT_VALVE_VALVE_STATE_VAL_IDX,
  VENT_VALVE_WAKEN_UP_IDX,
  VENT_VALVE_WAKEN_UP_VAL_IDX,
  VENT_VALVE_MAX_IDX
} rosalia_vent_valve_service_idxs;

typedef enum {
  LOADCELL_WEIGHT_IDX,
  LOADCELL_WEIGHT_VAL_IDX,
  LOADCELL_PRESSURE_IDX,
  LOADCELL_PRESSURE_VAL_IDX,
  LOACELL_MAX_IDX
} rosalia_loadcell_service_idxs;

typedef enum {
  FILLING_STATION_VALVE_STATE_IDX,
  FILLING_STATION_VALVE_STATE_VAL_IDX,
  FILLING_STATION_PRESSURE_IDX,
  FILLING_STATION_PRESSURE_VAL_IDX,
  FILLING_STATION_THERMOCOUPLES_IDX,
  FILLING_STATION_THERMOCOUPLES_VAL_IDX,
  FILLING_STATION_THERMISTORS_IDX,
  FILLING_STATION_THERMISTORS_VAL_IDX,
  FILLING_STATION_MAX_IDX
} rosalia_filling_station_service_idxs;

static uint8_t adv_config_done = 0;

static uint8_t adv_service_uuid128[32] = BLE_UUID_CONFIG_DEFAULT();

static esp_ble_adv_data_t adv_data = BLE_ADV_DATA_CONFIG_DEFAULT();

static esp_ble_adv_data_t scan_rsp_data = BLE_SCAN_RSP_DATA_CONFIG_DEFAULT();

static esp_ble_adv_params_t adv_params = BLE_ADV_PARAMS_CONFIG_DEFAULT();

static ble_gap_t ble_gap_conf = {.conf_type = BLE_GAP_BROADCASTER_CENTRAL,
                                 .event_handler_cb = gap_event_handler,
                                 .adv_data = adv_data,
                                 .scan_rsp_data = scan_rsp_data,
                                 .adv_params = adv_params};

static ble_gatts_t ble_gatt_conf = {
    .profiles_num = PROFILE_TOTAL_NUM,
    .event_handler_cb = gatts_event_handler,
    .profiles = {[PROFILE_A] = {.gatts_cb = gatt_profile_a_event_handler,
                                .gatts_if = ESP_GATT_IF_NONE}}};

static ble_config_t ble_conf = {.gap_config = &ble_gap_conf,
                                .gatt_config = &ble_gatt_conf,
                                .bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT()};

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

void ble_init_task(void* arg) {
  if (ble_init(&ble_conf) != BLE_OK) {
    ESP_LOGE(BLE_APP_TAG, "BLE init failed: %s", __func__);
    return;
  }

  ESP_LOGI(BLE_APP_TAG, "BLE init done");

  // ESP_LOGI(BLE_APP_TAG, "%d", ble_conf.bt_cfg);
  //   vTaskDelete(NULL);
}
