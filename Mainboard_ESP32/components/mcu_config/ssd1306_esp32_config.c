// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#include "ssd1306_esp32_config.h"

#define SSD1306_TAG "SSD1306_ESP32"

static mcu_i2c_config_t* i2c_config;

void ssd1306_esp32_config_mount_i2c_config(mcu_i2c_config_t* _i2c_config) {
  i2c_config = _i2c_config;
}

bool _ssd1306_i2c_master_write_byte(uint8_t _data, bool _ack_en) {
  return i2c_master_write_byte((*i2c_config).cmd, _data, _ack_en) == ESP_OK
             ? true
             : false;
}

bool _ssd1306_i2c_master_write(const uint8_t* _data, size_t _data_len,
                               bool _ack_en) {
  return i2c_master_write((*i2c_config).cmd, _data, _data_len, _ack_en) ==
                 ESP_OK
             ? true
             : false;
}

bool _ssd1306_i2c_master_start(ssd1306_i2c_cmd_handle_t cmd) {
  return i2c_master_start(cmd) == ESP_OK ? true : false;
}

bool _ssd1306_i2c_master_stop(ssd1306_i2c_cmd_handle_t cmd) {
  return i2c_master_stop(cmd) == ESP_OK ? true : false;
}

bool _ssd1306_i2c_master_cmd_begin(ssd1306_i2c_cmd_handle_t cmd,
                                   uint16_t ticks_to_wait) {
  return i2c_master_cmd_begin((*i2c_config).port, cmd, ticks_to_wait) == ESP_OK
             ? true
             : false;
}

ssd1306_i2c_cmd_handle_t _ssd1306_i2c_cmd_link_create() {
  return i2c_cmd_link_create();
}

void _ssd1306_i2c_cmd_link_delete() {
  return i2c_cmd_link_delete((*i2c_config).cmd);
}

void _ssd1306_delay(size_t _ms) { vTaskDelay(pdMS_TO_TICKS(_ms)); }

void _ssd1306_log(const ssd1306_log_level_t level, const char* tag,
                  char* info) {
  switch (level) {
    case SSD1306_NONE:
      break;
    case SSD1306_ERROR:
      ESP_LOGE(tag, "%s", info);
      break;
    case SSD1306_INFO:
      ESP_LOGI(tag, "%s", info);
      break;
    case SSD1306_DEBUG:
      ESP_LOGD(tag, "%s", info);
      break;
    default:
      ESP_LOGE(SSD1306_TAG, "Unknown debug level, %s", __func__);
  }
}
