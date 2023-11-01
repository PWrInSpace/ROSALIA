// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#include "app_init_task.h"

#define TAG "APP_INIT_TASK"

#define I2C_MASTER_FREQ_HZ 400000  // TODO(Glibus): sdkconig.h should have it

static adc_oneshot_unit_handle_t adc1_handle;

static ROSALIA_devices_t devices_config = {
    .twai = MCU_TWAI_DEFAULT_CONFIG(),
    .voltage_measure = MCU_VOLTAGE_MEASURE_DEFAULT_CONFIG(),
    .i2c = MCU_I2C_DEFAULT_CONFIG(),
    .oled_display = MCU_SSD1306_DEFAULT_CONFIG(),
    .rgb_led = MCU_RGB_LED_DRIVER_DEFAULT_CONFIG(),
    .buzzer = MCU_BUZZER_DRIVER_DEFAULT_CONFIG(),
};

void app_init_task(void* pvParameters) {
  ESP_LOGI(TAG, "Initializing devices");

  devices_config.voltage_measure.oneshot_unit_handle = &adc1_handle;

  esp_err_t ret = devices_init(&devices_config);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "Devices initialization failed");
    vTaskDelete(NULL);
  }

  for (int i = 0; i < 3; i++) {
    // ESP_LOGI(TAG, "%d", &devices_config->voltage_measure.adc_chan[i]);
  }

  xTaskCreatePinnedToCore(user_interface_task, "user_interface_task", 4096,
                          (void*) &devices_config, 1, NULL, 1);
}
