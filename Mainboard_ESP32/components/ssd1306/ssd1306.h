// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "font8x8_basic.h"
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

/*!
 * \brief enum for scroll type
 */
typedef enum {
  SCROLL_RIGHT = 1,
  SCROLL_LEFT = 2,
  SCROLL_DOWN = 3,
  SCROLL_UP = 4,
  SCROLL_STOP = 5
} ssd1306_scroll_type_t;

/*!
 * \brief enum for log level
 */
typedef enum {
  SSD1306_NONE,
  SSD1306_ERROR,
  SSD1306_INFO,
  SSD1306_DEBUG,
} ssd1306_log_level_t;

typedef void* ssd1306_i2c_cmd_handle_t;

typedef bool (*ssd1306_i2c_master_write_byte)(ssd1306_i2c_cmd_handle_t cmd,
                                              uint8_t _data, bool _ack_en);
typedef bool (*ssd1306_i2c_master_write)(ssd1306_i2c_cmd_handle_t cmd,
                                         const uint8_t* _data, size_t _data_len,
                                         bool _ack_en);
typedef bool (*ssd1306_i2c_master_start)(ssd1306_i2c_cmd_handle_t cmd);
typedef bool (*ssd1306_i2c_master_stop)();
typedef bool (*ssd1306_i2c_master_cmd_begin)(ssd1306_i2c_cmd_handle_t cmd,
                                             uint16_t ticks_to_wait);
typedef ssd1306_i2c_cmd_handle_t (*ssd1306_i2c_cmd_link_create)();
typedef void (*ssd1306_i2c_cmd_link_delete)(ssd1306_i2c_cmd_handle_t cmd);
typedef void (*ssd1306_delay)(size_t _ms);
typedef void (*ssd1306_log)(const ssd1306_log_level_t level, const char* tag,
                            char* info);

/*!
 * \brief SSD1306 display page structure
 */
typedef struct {
  uint8_t segments[OLED_BUFFER_SIZE];
} PAGE_t;

/*!
 * \brief SSD1306 display structure
 */
typedef struct {
  ssd1306_i2c_master_write_byte _i2c_master_write_byte;
  ssd1306_i2c_master_write _i2c_master_write;
  ssd1306_i2c_master_start _i2c_master_start;
  ssd1306_i2c_master_stop _i2c_master_stop;
  ssd1306_i2c_master_cmd_begin _i2c_master_cmd_begin;
  ssd1306_i2c_cmd_link_create _i2c_cmd_link_create;
  ssd1306_i2c_cmd_link_delete _i2c_cmd_link_delete;
  ssd1306_delay _delay;
  ssd1306_log _log;
  uint8_t i2c_master_write_flag;
  uint8_t width;
  uint8_t height;
  int pages;
  bool scroll_enable;
  int scroll_start;
  int scroll_end;
  int scroll_direction;
  PAGE_t screen_pages[8];
  bool flip;
  uint8_t i2c_address;
} ssd1306_t;

/*!
 * \brief Initialize SSD1306 display
 * \param[in] ssd SSD1306 display structure
 * \param[in] width Display width
 * \param[in] height Display height
 */
void ssd1306_init(ssd1306_t* ssd, uint8_t width, uint8_t height);

/*!
 * \brief Display whole image from buffer
 * \param[in] ssd SSD1306 display structure
 */
void ssd1306_show_buffer(ssd1306_t* ssd);

/*!
 * \brief Set buffer for display from array to pages
 * \param[in] ssd SSD1306 display structure
 * \param[in] buffer Buffer to set
 */
void ssd1306_set_buffer(ssd1306_t* ssd, uint8_t* buffer);

/*!
 * \brief Get buffer from display and save to buffer array
 * \param[in] ssd SSD1306 display structure
 * \param[out] buffer Buffer to get
 */
void ssd1306_get_buffer(ssd1306_t* ssd, uint8_t* buffer);

/*!
 * \brief Display image from buffer
 * \param[in] ssd SSD1306 display structure
 * \param[in] page Page to display
 * \param[in] seg Segment to display
 * \param[in] images Image to display
 * \param[in] width Image width
 */
void ssd1306_display_image(ssd1306_t* ssd, int page, int seg, uint8_t* images,
                           uint8_t width);

/*!
 * \brief Display text on display
 * \param[in] ssd SSD1306 display structure
 * \param[in] page Page to display
 * \param[in] text Text to display
 * \param[in] text_len Text length
 * \param[in] invert Invert text
 */
void ssd1306_display_text(ssd1306_t* ssd, int page, char* text, int text_len,
                          bool invert);

/*!
 * \brief Display text on display with x3 size
 * \param[in] ssd SSD1306 display structure
 * \param[in] page Page to display
 * \param[in] text Text to display
 * \param[in] text_len Text length
 * \param[in] invert Invert text
 */
void ssd1306_display_text_x3(ssd1306_t* ssd, int page, char* text, int text_len,
                             bool invert);

/*!
 * \brief Clear the whole screen
 * \param[in] ssd SSD1306 display structure
 * \param[in] invert Invert screen
 */
void ssd1306_clear_screen(ssd1306_t* ssd, bool invert);

/*!
 * \brief Clear the whole screen
 * \param[in] ssd SSD1306 display structure
 * \param[in] page Page to clear
 * \param[in] invert Invert screen
 */
void ssd1306_clear_line(ssd1306_t* ssd, int page, bool invert);

/*!
 * \brief Set contrast of display
 * \param[in] ssd SSD1306 display structure
 * \param[in] contrast Contrast value
 */
void ssd1306_set_contrast(ssd1306_t* ssd, int contrast);

/*!
 * \brief Perform software scroll of pixels
 * \param[in] ssd SSD1306 display structure
 * \param[in] start Start page
 * \param[in] end End page
 */
void ssd1306_software_scroll(ssd1306_t* ssd, int start, int end);

/*!
 * \brief Perform hardware scroll of text
 * \param[in] ssd SSD1306 display structure
 * \param[in] text Text to scroll
 * \param[in] text_len Text length
 * \param[in] invert Invert text
 */
void ssd1306_scroll_text(ssd1306_t* ssd, char* text, int text_len, bool invert);

/*!
 * \brief Clear image with scroll
 * \param[in] ssd SSD1306 display structure
 */
void ssd1306_scroll_clear(ssd1306_t* ssd);

/*!
 * \brief Perform hardware scroll of pixels
 * \param[in] ssd SSD1306 display structure
 * \param[in] scroll Scroll type
 */
void ssd1306_hardware_scroll(ssd1306_t* ssd, ssd1306_scroll_type_t scroll);

/*!
 * \brief Wrap around image through scroll
 * \param[in] ssd SSD1306 display structure
 * \param[in] scroll Scroll type
 * \param[in] start Start page
 * \param[in] end End page
 * \param[in] delay Delay between scroll
 */
void ssd1306_wrap_arround(ssd1306_t* ssd, ssd1306_scroll_type_t scroll,
                          int start, int end, int8_t delay);
/*!
 * \brief Show bitmap on display
 * \param[in] ssd SSD1306 display structure
 * \param[in] xpos X position
 * \param[in] ypos Y position
 * \param[in] bitmap Bitmap to display
 * \param[in] width Bitmap width
 * \param[in] height Bitmap height
 * \param[in] invert Invert bitmap
 */
void ssd1306_show_bitmap(ssd1306_t* ssd, int xpos, int ypos, uint8_t* bitmap,
                         uint8_t width, uint8_t height, bool invert);

/*!
 * \brief Save pixel to internal buffer, without displaying
 * \param[in] ssd SSD1306 display structure
 * \param[in] xpos X position
 * \param[in] ypos Y position
 * \param[in] invert Invert pixel
 */
void _ssd1306_pixel(ssd1306_t* ssd, int xpos, int ypos, bool invert);
/*!
 * \brief Save a line to internal buffer, without displaying
 * \param[in] ssd SSD1306 display structure
 * \param[in] x1 X1 position
 * \param[in] y1 Y1 position
 * \param[in] x2 X2 position
 * \param[in] y2 Y2 position
 * \param[in] invert Invert line
 */
void _ssd1306_line(ssd1306_t* ssd, int x1, int y1, int x2, int y2, bool invert);
/*!
 * \brief Invert a buffer
 * \param[in] buf Buffer to invert, after this operation buffer will be inverted
 * \param[in] blen Buffer length
 */
void ssd1306_invert(uint8_t* buf, size_t blen);
/*!
 * \brief Flip a buffer
 * \param[in] buf Buffer to flip, after this operation buffer will be flipped
 * \param[in] blen Buffer length
 */
void ssd1306_flip(uint8_t* buf, size_t blen);
/*!
 * \brief Copy a bit from one byte to another
 * \param[in] src Source byte
 * \param[in] srcBits Source bit position
 * \param[in] dst Destination byte
 * \param[in] dstBits Destination bit position
 */
uint8_t ssd1306_copy_bit(uint8_t src, int srcBits, uint8_t dst, int dstBits);
/*!
 * \brief Rotate a byte
 * \param[in] ch1 Byte to rotate
 * \return Rotated byte
 */
uint8_t ssd1306_rotate_byte(uint8_t ch1);
/*!
 * \brief Perform a fade out effect
 * \param[in] ssd SSD1306 display structure
 */
void ssd1306_fadeout(ssd1306_t* ssd);

/*!
 * \brief Init of the screen through i2c
 * \param[in] ssd SSD1306 display structure
 * \param[in] width Display width
 * \param[in] height Display height
 * \note This function is defined in ssd1306_i2c.c
 */
void ssd1306_i2c_init(ssd1306_t* ssd, uint8_t width, uint8_t height);

/*!
 * \brief Display an image through i2c
 * \param[in] ssd SSD1306 display structure
 * \param[in] page Page to display
 * \param[in] seg Segment to display
 * \param[in] images Image to display
 * \param[in] width Image width
 * \note This function is defined in ssd1306_i2c.c
 */
void ssd1306_i2c_display_image(ssd1306_t* ssd, int page, int seg,
                               uint8_t* images, uint8_t width);

/*!
 * \brief Set the display contrast through i2c
 * \param[in] ssd SSD1306 display structure
 * \param[in] contrast Contrast to set
 * \note This function is defined in ssd1306_i2c.c
 */
void ssd1306_i2c_set_contrast(ssd1306_t* ssd, int contrast);

/*!
 * \brief Perform hardware scroll of pixels through i2c
 * \param[in] ssd SSD1306 display structure
 * \param[in] scroll Scroll type
 * \note This function is defined in ssd1306_i2c.c
 */
void ssd1306_i2c_hardware_scroll(ssd1306_t* ssd, ssd1306_scroll_type_t scroll);
