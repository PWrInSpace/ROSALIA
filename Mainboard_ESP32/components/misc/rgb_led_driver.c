// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#include "rgb_led_driver.h"

#define RGB_LED_DRIVER_TAG "LED_Driver"

esp_err_t rgb_led_driver_init(rgb_led_driver_t *rgb_led_drv,
                              ledc_timer_bit_t ledc_duty_res,
                              uint32_t ledc_freq) {
  if (led_timer_init(&rgb_led_drv->led_drv[0], ledc_duty_res, ledc_freq) !=
      ESP_OK) {
    ESP_LOGE(RGB_LED_DRIVER_TAG, "LED timer init failed");
    return ESP_FAIL;
  }
  for (uint8_t i = 0; i < MAX_COLOR_INDEX; ++i) {
    rgb_led_drv->led_drv[i].max_duty = rgb_led_drv->max_duty;
    if (led_driver_init(&rgb_led_drv->led_drv[i]) != ESP_OK) {
      ESP_LOGE(RGB_LED_DRIVER_TAG, "LED driver init failed");
      return ESP_FAIL;
    }
  }
  return ESP_OK;
}

esp_err_t rgb_led_update_duty_cycle(rgb_led_driver_t *rgb_led_drv,
                                    uint32_t duty[MAX_COLOR_INDEX]) {
  for (uint8_t i = 0; i < MAX_COLOR_INDEX; ++i) {
    if (led_update_duty_cycle(&rgb_led_drv->led_drv[i], duty[i]) != ESP_OK) {
      ESP_LOGE(RGB_LED_DRIVER_TAG, "LED driver update duty cycle failed");
      return ESP_FAIL;
    }
  }
  ESP_LOGI(RGB_LED_DRIVER_TAG, "Updated duty cycle to %d %d %d", duty[0],
           duty[1], duty[2]);
  return ESP_OK;
}

esp_err_t rgb_led_toggle(rgb_led_driver_t *rgb_led_drv, led_state_t toggle) {
  for (uint8_t i = 0; i < MAX_COLOR_INDEX; ++i) {
    if (led_toggle(&rgb_led_drv->led_drv[i], toggle) != ESP_OK) {
      ESP_LOGE(RGB_LED_DRIVER_TAG, "LED driver toggle failed");
      return ESP_FAIL;
    }
  }
  ESP_LOGI(RGB_LED_DRIVER_TAG, "Toggled LED to %d", toggle);
  return ESP_OK;
}
