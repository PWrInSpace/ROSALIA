// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#include "i2c_config.h"
#include "ssd1306.h"

void ssd1306_esp32config_mount_i2c_config(i2c_config_t* _i2c_config);

bool _ssd1306_i2c_master_write_byte(uint8_t _data, bool _ack_en);

bool _ssd1306_i2c_master_write(const uint8_t* _data, size_t _data_len,
                               bool _ack_en);

bool _ssd1306_i2c_master_start(ssd1306_i2c_cmd_handle_t cmd);

bool _ssd1306_i2c_master_stop(ssd1306_i2c_cmd_handle_t cmd);

bool _ssd1306_i2c_master_cmd_begin(ssd1306_i2c_cmd_handle_t cmd,
                                   uint16_t ticks_to_wait);

// TODO(Glibus): tu sie moze wyjebac
ssd1306_i2c_cmd_handle_t _ssd1306_i2c_cmd_link_create();

void _ssd1306_i2c_cmd_link_delete();

void _ssd1306_delay(size_t _ms);

void _ssd1306_log(const ssd1306_debug_level_t level, const char* tag,
                  char* info);
