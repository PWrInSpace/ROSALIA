// Copyright 2022 PWrInSpace

#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ble.h"
#define TAG "MAIN"

void app_main(void) {
  ESP_LOGI(TAG, "Hello world!");
  vTaskDelay(pdMS_TO_TICKS(1000));
  ESP_LOGI(TAG, "https://youtu.be/5g2hT4GmAGU");
  vTaskDelay(pdMS_TO_TICKS(1000));
  ESP_LOGI(TAG, "Starting BLE");
  ble_init_task(NULL);
  while(1){
    ESP_LOGI(TAG, "Main loop");
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
  // vTaskDelete(NULL);
}
