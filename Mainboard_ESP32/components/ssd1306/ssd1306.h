// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#pragma once

#include "driver/spi_master.h"

/*!
 * \file ssd1306.h
 * \brief SSD1306 OLED display driver through I2C
 */

// Following definitions are bollowed from
// http://robotcantalk.blogspot.com/2015/03/interfacing-arduino-with-ssd1306-driven.html

/* Control byte for i2c
Co : bit 8 : Continuation Bit
 * 1 = no-continuation (only one byte to follow)
 * 0 = the controller should expect a stream of bytes.
D/C# : bit 7 : Data/Command Select bit
 * 1 = the next byte or byte stream will be Data.
 * 0 = a Command byte or byte stream will be coming up next.
 Bits 6-0 will be all zeros.
Usage:
0x80 : Single Command byte
0x00 : Command Stream
0xC0 : Single Data byte
0x40 : Data Stream
*/
#define OLED_CONTROL_BYTE_CMD_SINGLE 0x80
#define OLED_CONTROL_BYTE_CMD_STREAM 0x00
#define OLED_CONTROL_BYTE_DATA_SINGLE 0xC0
#define OLED_CONTROL_BYTE_DATA_STREAM 0x40

// Fundamental commands (pg.28)
#define OLED_CMD_SET_CONTRAST 0x81  // follow with 0x7F
#define OLED_CMD_DISPLAY_RAM 0xA4
#define OLED_CMD_DISPLAY_ALLON 0xA5
#define OLED_CMD_DISPLAY_NORMAL 0xA6
#define OLED_CMD_DISPLAY_INVERTED 0xA7
#define OLED_CMD_DISPLAY_OFF 0xAE
#define OLED_CMD_DISPLAY_ON 0xAF

// Addressing Command Table (pg.30)
#define OLED_CMD_SET_MEMORY_ADDR_MODE 0x20
#define OLED_CMD_SET_HORI_ADDR_MODE 0x00  // Horizontal Addressing Mode
#define OLED_CMD_SET_VERT_ADDR_MODE 0x01  // Vertical Addressing Mode
#define OLED_CMD_SET_PAGE_ADDR_MODE 0x02  // Page Addressing Mode
#define OLED_CMD_SET_COLUMN_RANGE \
  0x21  // can be used only in HORZ/VERT mode - follow with 0x00 and 0x7F =
        // COL127
#define OLED_CMD_SET_PAGE_RANGE \
  0x22  // can be used only in HORZ/VERT mode - follow with 0x00 and 0x07 =
        // PAGE7

// Hardware Config (pg.31)
#define OLED_CMD_SET_DISPLAY_START_LINE 0x40
#define OLED_CMD_SET_SEGMENT_REMAP_0 0xA0
#define OLED_CMD_SET_SEGMENT_REMAP_1 0xA1
#define OLED_CMD_SET_MUX_RATIO 0xA8  // follow with 0x3F = 64 MUX
#define OLED_CMD_SET_COM_SCAN_MODE 0xC8
#define OLED_CMD_SET_DISPLAY_OFFSET 0xD3  // follow with 0x00
#define OLED_CMD_SET_COM_PIN_MAP 0xDA     // follow with 0x12
#define OLED_CMD_NOP 0xE3                 // NOP

// Timing and Driving Scheme (pg.32)
#define OLED_CMD_SET_DISPLAY_CLK_DIV 0xD5  // follow with 0x80
#define OLED_CMD_SET_PRECHARGE 0xD9        // follow with 0xF1
#define OLED_CMD_SET_VCOMH_DESELCT 0xDB    // follow with 0x30

// Charge Pump (pg.62)
#define OLED_CMD_SET_CHARGE_PUMP 0x8D  // follow with 0x14

// Scrolling Commands
#define OLED_CMD_HORIZONTAL_RIGHT 0x26
#define OLED_CMD_HORIZONTAL_LEFT 0x27
#define OLED_CMD_CONTINUOUS_SCROLL 0x29
#define OLED_CMD_DEACTIVE_SCROLL 0x2E
#define OLED_CMD_ACTIVE_SCROLL 0x2F
#define OLED_CMD_VERTICAL 0xA3

// I2C Address
#define I2CAddress 0x3C  // TODO(Glibus): Move this to kconfig_projbuild

// Internal buffer size
#define OLED_BUFFER_SIZE 128

typedef enum {
  SCROLL_RIGHT = 1,
  SCROLL_LEFT = 2,
  SCROLL_DOWN = 3,
  SCROLL_UP = 4,
  SCROLL_STOP = 5
} ssd1306_scroll_type_t;

typedef enum {
  SSD1306_NONE,
  SSD1306_ERROR,
  SSD1306_INFO,
  SSD1306_DEBUG,
} ssd1306_debug_level_t;

typedef bool (*ssd1306_i2c_master_write_byte)(uint8_t _data, bool _ack_en);
typedef bool (*ssd1306_i2c_master_write)(const uint8_t* _data, size_t _data_len,
                                         bool _ack_en);
typedef bool (*ssd1306_i2c_master_start)();
typedef bool (*ssd1306_i2c_master_stop)();
typedef bool (*ssd1306_i2c_master_cmd_begin)(uint16_t ticks_to_wait);
typedef bool (*ssd1306_i2c_cmd_link_delete)();
typedef void (*ssd1306_delay)(size_t _ms);
typedef void (*ssd1306_log)(const ssd1306_debug_level_t level, char* info);

typedef struct {
  bool _valid;  // Not using it anymore
  int _segLen;  // Not using it anymore
  uint8_t _segs[OLED_BUFFER_SIZE];
} PAGE_t;

// typedef struct {
//   int _address;
//   uint8_t width;
//   uint8_t height;
//   int pages;
//   int _dc;
//   spi_device_handle_t _SPIHandle;
//   bool scroll_enable;
//   int scroll_start;
//   int scroll_end;
//   int scroll_direction;
//   PAGE_t screen_pages[8];
//   bool flip;
// } ssd1306_t;

typedef struct {
  uint8_t width;
  uint8_t height;
  int pages;
  spi_device_handle_t _SPIHandle;  // TODO(Glibus): replace with function ptrs
  bool scroll_enable;
  int scroll_start;
  int scroll_end;
  int scroll_direction;
  PAGE_t screen_pages[8];
  bool flip;
  uint8_t i2c_address;
} ssd1306_t;

void ssd1306_init(ssd1306_t* ssd, uint8_t width, uint8_t height);

void ssd1306_show_buffer(ssd1306_t* ssd);
void ssd1306_set_buffer(ssd1306_t* ssd, uint8_t* buffer);
void ssd1306_get_buffer(ssd1306_t* ssd, uint8_t* buffer);
void ssd1306_display_image(ssd1306_t* ssd, int page, int seg, uint8_t* images,
                           uint8_t width);
void ssd1306_display_text(ssd1306_t* ssd, int page, char* text, int text_len,
                          bool invert);
void ssd1306_display_text_x3(ssd1306_t* ssd, int page, char* text, int text_len,
                             bool invert);
void ssd1306_clear_screen(ssd1306_t* ssd, bool invert);
void ssd1306_clear_line(ssd1306_t* ssd, int page, bool invert);
void ssd1306_contrast(ssd1306_t* ssd, int contrast);
void ssd1306_software_scroll(ssd1306_t* ssd, int start, int end);
void ssd1306_scroll_text(ssd1306_t* ssd, char* text, int text_len, bool invert);
void ssd1306_scroll_clear(ssd1306_t* ssd);
void ssd1306_hardware_scroll(ssd1306_t* ssd, ssd1306_scroll_type_t scroll);
void ssd1306_wrap_arround(ssd1306_t* ssd, ssd1306_scroll_type_t scroll,
                          int start, int end, int8_t delay);
void ssd1306_bitmaps(ssd1306_t* ssd, int xpos, int ypos, uint8_t* bitmap,
                     uint8_t width, uint8_t height, bool invert);
void _ssd1306_pixel(ssd1306_t* ssd, int xpos, int ypos, bool invert);
void _ssd1306_line(ssd1306_t* ssd, int x1, int y1, int x2, int y2, bool invert);
void ssd1306_invert(uint8_t* buf, size_t blen);
void ssd1306_flip(uint8_t* buf, size_t blen);
uint8_t ssd1306_copy_bit(uint8_t src, int srcBits, uint8_t dst, int dstBits);
uint8_t ssd1306_rotate_byte(uint8_t ch1);
void ssd1306_fadeout(ssd1306_t* ssd);
void ssd1306_dump_page(ssd1306_t* ssd, int page, int seg);

void i2c_master_init(ssd1306_t* ssd, int16_t sda, int16_t scl, int16_t reset);
void i2c_init(ssd1306_t* ssd, uint8_t width, uint8_t height);
void i2c_display_image(ssd1306_t* ssd, int page, int seg, uint8_t* images,
                       uint8_t width);
void i2c_contrast(ssd1306_t* ssd, int contrast);
void i2c_hardware_scroll(ssd1306_t* ssd, ssd1306_scroll_type_t scroll);
