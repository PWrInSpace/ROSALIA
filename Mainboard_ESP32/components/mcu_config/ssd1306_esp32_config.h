// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#include "mcu_i2c_config.h"
#include "ssd1306.h"

#define MCU_SSD1306_DEFAULT_CONFIG()                              \
  {                                                               \
    ._i2c_master_write_byte = _ssd1306_i2c_master_write_byte,     \
    ._i2c_master_write = _ssd1306_i2c_master_write,               \
    ._i2c_master_start = _ssd1306_i2c_master_start,               \
    ._i2c_master_stop = _ssd1306_i2c_master_stop,                 \
    ._i2c_master_cmd_begin = _ssd1306_i2c_master_cmd_begin,       \
    ._i2c_cmd_link_create = _ssd1306_i2c_cmd_link_create,         \
    ._i2c_cmd_link_delete = _ssd1306_i2c_cmd_link_delete,         \
    ._delay = _ssd1306_delay, ._log = _ssd1306_log, .width = 128, \
    .height = 64, .pages = 1, .i2c_address = 0x3c,                \
  }

void ssd1306_esp32_config_mount_i2c_config(mcu_i2c_config_t* _i2c_config);

bool _ssd1306_i2c_master_write_byte(ssd1306_i2c_cmd_handle_t cmd, uint8_t _data,
                                    bool _ack_en);

bool _ssd1306_i2c_master_write(ssd1306_i2c_cmd_handle_t cmd,
                               const uint8_t* _data, size_t _data_len,
                               bool _ack_en);

bool _ssd1306_i2c_master_start(ssd1306_i2c_cmd_handle_t cmd);

bool _ssd1306_i2c_master_stop(ssd1306_i2c_cmd_handle_t cmd);

bool _ssd1306_i2c_master_cmd_begin(ssd1306_i2c_cmd_handle_t cmd,
                                   uint16_t ticks_to_wait);

ssd1306_i2c_cmd_handle_t _ssd1306_i2c_cmd_link_create();

void _ssd1306_i2c_cmd_link_delete(ssd1306_i2c_cmd_handle_t cmd);

void _ssd1306_delay(size_t _ms);

void _ssd1306_log(const ssd1306_log_level_t level, const char* tag, char* info);
