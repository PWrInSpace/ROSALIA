// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#include "i2c_config.h"
#include "ssd1306.h"


bool _ssd1306_i2c_master_write_byte(uint8_t _data, bool _ack_en);

bool _ssd1306_i2c_master_write(const uint8_t* _data,
                           size_t _data_len; bool _ack_en);

bool _ssd1306_i2c_master_start();

bool _ssd1306_i2c_master_stop();

bool _ssd1306_i2c_master_cmd_begin(uint16_t ticks_to_wait);

bool _ssd1306_i2c_cmd_link_delete();

void _ssd1306_delay(size_t _ms);

void _ssd1306_log(const ssd1306_debug_level_t level, char* info);
