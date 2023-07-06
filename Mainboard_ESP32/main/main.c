// Copyright 2022 PWrInSpace

#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "freertos/task.h"
#define TAG "MAIN"

void app_main(void) {
  ESP_LOGI(TAG, "Hello world!");
  vTaskDelay(pdMS_TO_TICKS(1000));
  ESP_LOGI(TAG, "Goodbye world!");
  vTaskDelete(NULL);
}
