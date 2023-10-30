// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#include "user_interface.h"

#define USER_INTERFACE_TAG "USER_INTERFACE"

void user_interface_task(void* pvParameters) {
  ROSALIA_devices_t* devices_config = (ROSALIA_devices_t*)pvParameters;

  int center, top, bottom;
  ESP_LOGI(USER_INTERFACE_TAG, "RGB LED driver initialized");
  led_state_t toggle = LED_OFF;
  for (;;) {
    toggle = !toggle;
    rgb_led_toggle(&devices_config->rgb_led, toggle);
    ESP_LOGI(USER_INTERFACE_TAG, "%f",
             voltage_measure_read_voltage(&devices_config->voltage_measure, 0));
    ESP_LOGI(USER_INTERFACE_TAG, "%f",
             voltage_measure_read_voltage(&devices_config->voltage_measure, 1));
    ESP_LOGI(USER_INTERFACE_TAG, "%f",
             voltage_measure_read_voltage(&devices_config->voltage_measure, 2));
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
