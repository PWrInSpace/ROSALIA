// Copyright 2023 PWr in Space, Krzysztof Gliwiński
#include "ble_api.h"
#include "ble_gap_conf.h"
#include "ble_gatt_conf.h"

typedef enum {
  PROFILE_A,
  PROFILE_B,
  PROFILE_TOTAL_NUM
} ble_gap_profile_names_t;

void gap_event_handler(esp_gap_ble_cb_event_t event,
                       esp_ble_gap_cb_param_t* param);

void ble_init_task(void* arg);
