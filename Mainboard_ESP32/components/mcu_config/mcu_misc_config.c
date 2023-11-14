// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#include "mcu_misc_config.h"

#define MISC_TAG "MCU_MISC_CONFIG"

esp_err_t misc_init(buzzer_driver_t *buzzer, rgb_led_driver_t *rgb_led) {
  esp_err_t err = ESP_OK;
  err = rgb_led_driver_init(rgb_led, RGB_LED_DUTY_RES, RGB_LED_FREQ_HZ);
  if (err != ESP_OK) {
    ESP_LOGE(MISC_TAG, "rgb_led_driver_init failed with error %d", err);
    return err;
  }
  err = buzzer_init(buzzer, BUZZER_DUTY_RES, BUZZER_FREQ_HZ);
  if (err != ESP_OK) {
    ESP_LOGE(MISC_TAG, "buzzer_init failed with error %d", err);
    return err;
  }
  return err;
}
