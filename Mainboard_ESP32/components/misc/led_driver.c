// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#include "led_driver.h"

esp_err_t led_driver_init(led_driver_t *led_drv, uint8_t led_gpio_num,
                     uint8_t ledc_channel_num, uint8_t ledc_timer_num) {
  ledc_timer_config_t ledc_timer = {
      .speed_mode = LEDC_MODE,
      .timer_num = ledc_timer_num,
      .duty_resolution = LEDC_DUTY_RES,
      .freq_hz = LEDC_FREQUENCY,  // Set output frequency at 5 kHz
      .clk_cfg = LEDC_AUTO_CLK};
  if (ledc_timer_config(&ledc_timer) != ESP_OK) {
    ESP_LOGE(LED_DRIVER_TAG, "Ledc timer config failed!");
    return ESP_FAIL;
  }

  ledc_channel_config_t ledc_channel = {.speed_mode = LEDC_MODE,
                                        .channel = ledc_channel_num,
                                        .timer_sel = ledc_timer_num,
                                        .intr_type = LEDC_INTR_DISABLE,
                                        .gpio_num = led_gpio_num,
                                        .duty = 0,  // Set duty to 0%
                                        .hpoint = 0};
  if (ledc_channel_config(&ledc_channel) != ESP_OK) {
    ESP_LOGE(LED_DRIVER_TAG, "LEDc channel config failed");
    return ESP_FAIL;
  }
  led_drv->led_gpio_num = led_gpio_num;
  led_drv->ledc_channel_num = ledc_channel_num;
  led_drv->ledc_timer_num = ledc_timer_num;
  return ESP_OK;
}

esp_err_t led_driver_update_duty_cycle(led_driver_t *led_drv, uint16_t duty) {
  if (duty > MAX_LED_DUTY) {
    ESP_LOGE(LED_DRIVER_TAG, "Duty too large, expected max. %d, actual: %d",
             MAX_LED_DUTY, duty);
    return ESP_FAIL;
  }
  if (ledc_set_duty(LEDC_MODE, led_drv->ledc_channel_num, duty) != ESP_OK) {
    ESP_LOGE(LED_DRIVER_TAG, "LEDc set duty failed");
    return ESP_FAIL;
  }

  if (ledc_update_duty(LEDC_MODE, led_drv->ledc_channel_num) != ESP_OK) {
    ESP_LOGE(LED_DRIVER_TAG, "LEDc Update duty failed");
    return ESP_FAIL;
  }

  return ESP_OK;
}
