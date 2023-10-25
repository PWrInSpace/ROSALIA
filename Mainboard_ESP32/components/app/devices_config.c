// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#include "devices_config.h"

#define TAG "DEVICES_CONFIG"

esp_err_t devices_init(ROSALIA_devices_t* devices) {
  esp_err_t ret = ESP_OK;
  twai_general_config_t twai_g_config = TWAI_GENERAL_CONFIG_DEFAULT(
      devices->twai.tx_gpio_num, devices->twai.rx_gpio_num, TWAI_MODE_NORMAL);
  twai_timing_config_t twai_t_config = TWAI_TIMING_CONFIG_500KBITS();
  twai_filter_config_t twai_f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
  ret |=
      twai_init(&devices->twai, &twai_g_config, &twai_t_config, &twai_f_config);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "TWAI initialization failed");
    return ret;
  }
  //   ret |= spi_init(&devices->spi);

  //   ret |= lora_init(&devices->lora);
  ret |= i2c_init(&devices->i2c);

  ssd1306_esp32_config_mount_i2c_config(&devices->i2c);
  ssd1306_init(&devices->oled_display, 128, 64);
  ssd1306_clear_screen(&devices->oled_display, false);
  ssd1306_set_contrast(&devices->oled_display, 0xff);
  ssd1306_display_text_x3(&devices->oled_display, 0, "Hello", 5, false);

  rgb_led_driver_init(&devices->rgb_led, LEDC_DUTY_RES, LEDC_FREQ_HZ);

  voltage_measure_init(&devices->voltage_measure);

  vTaskDelay(3000 / portTICK_PERIOD_MS);

  return ret;
}
