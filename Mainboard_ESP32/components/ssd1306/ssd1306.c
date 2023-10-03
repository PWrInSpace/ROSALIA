// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#include "ssd1306.h"

#include <string.h>

#include "esp_log.h"
#include "font8x8_basic.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG "SSD1306"

#define PACK8 __attribute__((aligned(__alignof__(uint8_t)), packed))

typedef union out_column_t {
  uint32_t u32;
  uint8_t u8[4];
} PACK8 out_column_t;

void ssd1306_init(ssd1306_t *ssd, uint8_t width, uint8_t height) {
  i2c_init(ssd, width, height);
  for (int i = 0; i < ssd->pages; i++) {
    memset(ssd->screen_pages[i]._segs, 0, OLED_BUFFER_SIZE);
  }
}

void ssd1306_show_buffer(ssd1306_t *ssd) {
  for (int page = 0; page < ssd->pages; page++) {
    i2c_display_image(ssd, page, 0, ssd->screen_pages[page]._segs, ssd->width);
  }
}

void ssd1306_set_buffer(ssd1306_t *ssd, uint8_t *buffer) {
  int index = 0;
  for (int page_num = 0; page_num < ssd->pages; page_num++) {
    memcpy(&ssd->screen_pages[page_num]._segs, &buffer[index], 128);
    index = index + 128;
  }
}

void ssd1306_get_buffer(ssd1306_t *ssd, uint8_t *buffer) {
  int index = 0;
  for (int page = 0; page < ssd->pages; page++) {
    memcpy(&buffer[index], &ssd->screen_pages[page]._segs, 128);
    index = index + 128;
  }
}

void ssd1306_display_image(ssd1306_t *ssd, int page, int seg, uint8_t *images,
                           uint8_t width) {
  i2c_display_image(ssd, page, seg, images, width);

  // Set to internal buffer
  memcpy(&ssd->screen_pages[page]._segs[seg], images, width);
}

void ssd1306_display_text(ssd1306_t *ssd, int page, char *text, int text_len,
                          bool invert) {
  if (page >= ssd->pages) {
    return;
  }
  int _text_len = text_len;
  if (_text_len > 16) {
    _text_len = 16;
  }

  uint8_t seg = 0;
  uint8_t image[8];
  for (uint8_t i = 0; i < _text_len; i++) {
    memcpy(image, font8x8_basic_tr[(uint8_t)text[i]], 8);
    if (invert) {
      ssd1306_invert(image, 8);
    }
    if (ssd->flip) {
      ssd1306_flip(image, 8);
    }
    ssd1306_display_image(ssd, page, seg, image, 8);

    seg = seg + 8;
  }
}

// by Coert Vonk
void ssd1306_display_text_x3(ssd1306_t *ssd, int page, char *text, int text_len,
                             bool invert) {
  if (page >= ssd->pages) {
    return;
  }
  int _text_len = text_len;
  if (_text_len > 5) {
    _text_len = 5;
  }

  uint8_t seg = 0;

  for (uint8_t nn = 0; nn < _text_len; nn++) {
    uint8_t const *const in_columns = font8x8_basic_tr[(uint8_t)text[nn]];

    // make the character 3x as high
    out_column_t out_columns[8];
    memset(out_columns, 0, sizeof(out_columns));

    for (uint8_t xx = 0; xx < 8; xx++) {  // for each column (x-direction)
      uint32_t in_bitmask = 0b1;
      uint32_t out_bitmask = 0b111;

      for (uint8_t yy = 0; yy < 8; yy++) {  // for pixel (y-direction)
        if (in_columns[xx] & in_bitmask) {
          out_columns[xx].u32 |= out_bitmask;
        }
        in_bitmask <<= 1;
        out_bitmask <<= 3;
      }
    }

    // render character in 8 column high pieces, making them 3x as wide
    for (uint8_t yy = 0; yy < 3;
         yy++) {  // for each group of 8 pixels high (y-direction)
      uint8_t image[24];
      for (uint8_t xx = 0; xx < 8; xx++) {  // for each column (x-direction)
        image[xx * 3 + 0] = image[xx * 3 + 1] = image[xx * 3 + 2] =
            out_columns[xx].u8[yy];
      }
      if (invert) {
        ssd1306_invert(image, 24);
      }
      if (ssd->flip) {
        ssd1306_flip(image, 24);
      }

      i2c_display_image(ssd, page + yy, seg, image, 24);

      memcpy(&ssd->screen_pages[page + yy]._segs[seg], image, 24);
    }
    seg = seg + 24;
  }
}

void ssd1306_clear_screen(ssd1306_t *ssd, bool invert) {
  char space[16];
  memset(space, 0x00, sizeof(space));
  for (int page = 0; page < ssd->pages; page++) {
    ssd1306_display_text(ssd, page, space, sizeof(space), invert);
  }
}

void ssd1306_clear_line(ssd1306_t *ssd, int page, bool invert) {
  char space[16];
  memset(space, 0x00, sizeof(space));
  ssd1306_display_text(ssd, page, space, sizeof(space), invert);
}

void ssd1306_contrast(ssd1306_t *ssd, int contrast) {
  i2c_contrast(ssd, contrast);
}

void ssd1306_software_scroll(ssd1306_t *ssd, int start, int end) {
  ESP_LOGD(TAG, "software_scroll start=%d end=%d pages=%d", start, end,
           ssd->pages);
  if (start < 0 || end < 0) {
    ssd->scroll_enable = false;
  } else if (start >= ssd->pages || end >= ssd->pages) {
    ssd->scroll_enable = false;
  } else {
    ssd->scroll_enable = true;
    ssd->scroll_start = start;
    ssd->scroll_end = end;
    ssd->scroll_direction = 1;
    if (start > end) {
      ssd->scroll_direction = -1;
    }
  }
}

void ssd1306_scroll_text(ssd1306_t *ssd, char *text, int text_len,
                         bool invert) {
  ESP_LOGD(TAG, "ssd->scroll_enable=%d", ssd->scroll_enable);
  if (ssd->scroll_enable == false) {
    return;
  }

  void (*func)(ssd1306_t *ssd, int page, int seg, uint8_t *images,
               uint8_t width);

  func = i2c_display_image;

  int srcIndex = ssd->scroll_end - ssd->scroll_direction;
  while (1) {
    int dstIndex = srcIndex + ssd->scroll_direction;
    ESP_LOGD(TAG, "srcIndex=%d dstIndex=%d", srcIndex, dstIndex);
    for (int seg = 0; seg < ssd->width; seg++) {
      ssd->screen_pages[dstIndex]._segs[seg] =
          ssd->screen_pages[srcIndex]._segs[seg];
    }
    (*func)(ssd, dstIndex, 0, ssd->screen_pages[dstIndex]._segs,
            sizeof(ssd->screen_pages[dstIndex]._segs));
    if (srcIndex == ssd->scroll_start) {
      break;
    }
    srcIndex = srcIndex - ssd->scroll_direction;
  }

  int _text_len = text_len;
  if (_text_len > 16) {
    _text_len = 16;
  }

  ssd1306_display_text(ssd, srcIndex, text, text_len, invert);
}

void ssd1306_scroll_clear(ssd1306_t *ssd) {
  ESP_LOGD(TAG, "ssd->scroll_enable=%d", ssd->scroll_enable);
  if (ssd->scroll_enable == false) {
    return;
  }

  int srcIndex = ssd->scroll_end - ssd->scroll_direction;
  while (1) {
    int dstIndex = srcIndex + ssd->scroll_direction;
    ESP_LOGD(TAG, "srcIndex=%d dstIndex=%d", srcIndex, dstIndex);
    ssd1306_clear_line(ssd, dstIndex, false);
    if (dstIndex == ssd->scroll_start) {
      break;
    }
    srcIndex = srcIndex - ssd->scroll_direction;
  }
}

void ssd1306_hardware_scroll(ssd1306_t *ssd, ssd1306_scroll_type_t scroll) {
  i2c_hardware_scroll(ssd, scroll);
}

// delay = 0 : display with no wait
// delay > 0 : display with wait
// delay < 0 : no display
void ssd1306_wrap_arround(ssd1306_t *ssd, ssd1306_scroll_type_t scroll,
                          int start, int end, int8_t delay) {
  if (scroll == SCROLL_RIGHT) {
    int _start = start;  // 0 to 7
    int _end = end;      // 0 to 7
    if (_end >= ssd->pages) {
      _end = ssd->pages - 1;
    }
    uint8_t wk;
    // for (int page=0;page<ssd->pages;page++) {
    for (int page = _start; page <= _end; page++) {
      wk = ssd->screen_pages[page]._segs[127];
      for (int seg = 127; seg > 0; seg--) {
        ssd->screen_pages[page]._segs[seg] =
            ssd->screen_pages[page]._segs[seg - 1];
      }
      ssd->screen_pages[page]._segs[0] = wk;
    }

  } else if (scroll == SCROLL_LEFT) {
    int _start = start;  // 0 to 7
    int _end = end;      // 0 to 7
    if (_end >= ssd->pages) {
      _end = ssd->pages - 1;
    }
    uint8_t wk;
    // for (int page=0;page<ssd->pages;page++) {
    for (int page = _start; page <= _end; page++) {
      wk = ssd->screen_pages[page]._segs[0];
      for (int seg = 0; seg < 127; seg++) {
        ssd->screen_pages[page]._segs[seg] =
            ssd->screen_pages[page]._segs[seg + 1];
      }
      ssd->screen_pages[page]._segs[127] = wk;
    }

  } else if (scroll == SCROLL_UP) {
    int _start = start;  // 0 to {width-1}
    int _end = end;      // 0 to {width-1}
    if (_end >= ssd->width) {
      _end = ssd->width - 1;
    }
    uint8_t wk0;
    uint8_t wk1;
    uint8_t wk2;
    uint8_t save[128];
    // Save pages 0
    for (int seg = 0; seg < 128; seg++) {
      save[seg] = ssd->screen_pages[0]._segs[seg];
    }
    // Page0 to Page6
    for (int page = 0; page < ssd->pages - 1; page++) {
      // for (int seg=0;seg<128;seg++) {
      for (int seg = _start; seg <= _end; seg++) {
        wk0 = ssd->screen_pages[page]._segs[seg];
        wk1 = ssd->screen_pages[page + 1]._segs[seg];
        if (ssd->flip) {
          wk0 = ssd1306_rotate_byte(wk0);
        }
        if (ssd->flip) {
          wk1 = ssd1306_rotate_byte(wk1);
        }
        if (seg == 0) {
          ESP_LOGD(TAG, "b page=%d wk0=%02x wk1=%02x", page, wk0, wk1);
        }
        wk0 = wk0 >> 1;
        wk1 = wk1 & 0x01;
        wk1 = wk1 << 7;
        wk2 = wk0 | wk1;
        if (seg == 0) {
          ESP_LOGD(TAG, "a page=%d wk0=%02x wk1=%02x wk2=%02x", page, wk0, wk1,
                   wk2);
        }
        if (ssd->flip) {
          wk2 = ssd1306_rotate_byte(wk2);
        }
        ssd->screen_pages[page]._segs[seg] = wk2;
      }
    }
    // Page7
    int pages = ssd->pages - 1;
    // for (int seg=0;seg<128;seg++) {
    for (int seg = _start; seg <= _end; seg++) {
      wk0 = ssd->screen_pages[pages]._segs[seg];
      wk1 = save[seg];
      if (ssd->flip) {
        wk0 = ssd1306_rotate_byte(wk0);
      }
      if (ssd->flip) {
        wk1 = ssd1306_rotate_byte(wk1);
      }
      wk0 = wk0 >> 1;
      wk1 = wk1 & 0x01;
      wk1 = wk1 << 7;
      wk2 = wk0 | wk1;
      if (ssd->flip) {
        wk2 = ssd1306_rotate_byte(wk2);
      }
      ssd->screen_pages[pages]._segs[seg] = wk2;
    }

  } else if (scroll == SCROLL_DOWN) {
    int _start = start;  // 0 to {width-1}
    int _end = end;      // 0 to {width-1}
    if (_end >= ssd->width) {
      _end = ssd->width - 1;
    }
    uint8_t wk0;
    uint8_t wk1;
    uint8_t wk2;
    uint8_t save[128];
    // Save pages 7
    int pages = ssd->pages - 1;
    for (int seg = 0; seg < 128; seg++) {
      save[seg] = ssd->screen_pages[pages]._segs[seg];
    }
    // Page7 to Page1
    for (int page = pages; page > 0; page--) {
      // for (int seg=0;seg<128;seg++) {
      for (int seg = _start; seg <= _end; seg++) {
        wk0 = ssd->screen_pages[page]._segs[seg];
        wk1 = ssd->screen_pages[page - 1]._segs[seg];
        if (ssd->flip) {
          wk0 = ssd1306_rotate_byte(wk0);
        }
        if (ssd->flip) {
          wk1 = ssd1306_rotate_byte(wk1);
        }
        if (seg == 0) {
          ESP_LOGD(TAG, "b page=%d wk0=%02x wk1=%02x", page, wk0, wk1);
        }
        wk0 = wk0 << 1;
        wk1 = wk1 & 0x80;
        wk1 = wk1 >> 7;
        wk2 = wk0 | wk1;
        if (seg == 0) {
          ESP_LOGD(TAG, "a page=%d wk0=%02x wk1=%02x wk2=%02x", page, wk0, wk1,
                   wk2);
        }
        if (ssd->flip) {
          wk2 = ssd1306_rotate_byte(wk2);
        }
        ssd->screen_pages[page]._segs[seg] = wk2;
      }
    }
    // Page0
    // for (int seg=0;seg<128;seg++) {
    for (int seg = _start; seg <= _end; seg++) {
      wk0 = ssd->screen_pages[0]._segs[seg];
      wk1 = save[seg];
      if (ssd->flip) {
        wk0 = ssd1306_rotate_byte(wk0);
      }
      if (ssd->flip) {
        wk1 = ssd1306_rotate_byte(wk1);
      }
      wk0 = wk0 << 1;
      wk1 = wk1 & 0x80;
      wk1 = wk1 >> 7;
      wk2 = wk0 | wk1;
      if (ssd->flip) {
        wk2 = ssd1306_rotate_byte(wk2);
      }
      ssd->screen_pages[0]._segs[seg] = wk2;
    }
  }

  if (delay >= 0) {
    for (int page = 0; page < ssd->pages; page++) {
      i2c_display_image(ssd, page, 0, ssd->screen_pages[page]._segs, 128);

      if (delay) {
        vTaskDelay(delay);
      }
    }
  }
}

void ssd1306_bitmaps(ssd1306_t *ssd, int xpos, int ypos, uint8_t *bitmap,
                     uint8_t width, uint8_t height, bool invert) {
  if ((width % 8) != 0) {
    ESP_LOGE(TAG, "width must be a multiple of 8");
    return;
  }
  width /= 8;
  uint8_t wk0;
  uint8_t wk1;
  uint8_t wk2;
  uint8_t page = (ypos / 8);
  uint8_t _seg = xpos;
  uint8_t dstBits = (ypos % 8);
  ESP_LOGD(TAG, "ypos=%d page=%d dstBits=%d", ypos, page, dstBits);
  int offset = 0;
  for (int i = 0; i < height;
       i++) {  // TODO(Glibus): Something may have broken here
    for (int index = 0; index < width; index++) {
      for (int srcBits = 7; srcBits >= 0; srcBits--) {
        wk0 = ssd->screen_pages[page]._segs[_seg];
        if (ssd->flip) {
          wk0 = ssd1306_rotate_byte(wk0);
        }

        wk1 = bitmap[index + offset];
        if (invert) {
          wk1 = ~wk1;
        }

        // wk2 = ssd1306_copy_bit(bitmap[index+offset], srcBits, wk0, dstBits);
        wk2 = ssd1306_copy_bit(wk1, srcBits, wk0, dstBits);
        if (ssd->flip) {
          wk2 = ssd1306_rotate_byte(wk2);
        }

        ESP_LOGD(TAG, "index=%d offset=%d page=%d _seg=%d, wk2=%02x", index,
                 offset, page, _seg, wk2);
        ssd->screen_pages[page]._segs[_seg] = wk2;
        _seg++;
      }
    }
    vTaskDelay(1);
    offset = offset + width;
    dstBits++;
    _seg = xpos;
    if (dstBits == 8) {
      page++;
      dstBits = 0;
    }
  }
  ssd1306_show_buffer(ssd);
}

// Set pixel to internal buffer. Not show it.
void _ssd1306_pixel(ssd1306_t *ssd, int xpos, int ypos, bool invert) {
  uint8_t screen_pages = (ypos / 8);
  uint8_t _bits = (ypos % 8);
  uint8_t _seg = xpos;
  uint8_t wk0 = ssd->screen_pages[screen_pages]._segs[_seg];
  uint8_t wk1 = 1 << _bits;
  ESP_LOGD(TAG, "ypos=%d screen_pages=%d _bits=%d wk0=0x%02x wk1=0x%02x", ypos,
           screen_pages, _bits, wk0, wk1);
  if (invert) {
    wk0 = wk0 & ~wk1;
  } else {
    wk0 = wk0 | wk1;
  }
  if (ssd->flip) {
    wk0 = ssd1306_rotate_byte(wk0);
  }
  ESP_LOGD(TAG, "wk0=0x%02x wk1=0x%02x", wk0, wk1);
  ssd->screen_pages[screen_pages]._segs[_seg] = wk0;
}

// Set line to internal buffer. Not show it.
void _ssd1306_line(ssd1306_t *ssd, int x1, int y1, int x2, int y2,
                   bool invert) {
  int i;
  int dx, dy;
  int sx, sy;
  int E;

  /* distance between two points */
  dx = (x2 > x1) ? x2 - x1 : x1 - x2;
  dy = (y2 > y1) ? y2 - y1 : y1 - y2;

  /* direction of two point */
  sx = (x2 > x1) ? 1 : -1;
  sy = (y2 > y1) ? 1 : -1;

  /* inclination < 1 */
  if (dx > dy) {
    E = -dx;
    for (i = 0; i <= dx; i++) {
      _ssd1306_pixel(ssd, x1, y1, invert);
      x1 += sx;
      E += 2 * dy;
      if (E >= 0) {
        y1 += sy;
        E -= 2 * dx;
      }
    }

    /* inclination >= 1 */
  } else {
    E = -dy;
    for (i = 0; i <= dy; i++) {
      _ssd1306_pixel(ssd, x1, y1, invert);
      y1 += sy;
      E += 2 * dx;
      if (E >= 0) {
        x1 += sx;
        E -= 2 * dy;
      }
    }
  }
}

void ssd1306_invert(uint8_t *buf, size_t blen) {
  uint8_t wk;
  for (int i = 0; i < blen; i++) {
    wk = buf[i];
    buf[i] = ~wk;
  }
}

// Flip upside down
void ssd1306_flip(uint8_t *buf, size_t blen) {
  for (int i = 0; i < blen; i++) {
    buf[i] = ssd1306_rotate_byte(buf[i]);
  }
}

uint8_t ssd1306_copy_bit(uint8_t src, int srcBits, uint8_t dst, int dstBits) {
  ESP_LOGD(TAG, "src=%02x srcBits=%d dst=%02x dstBits=%d", src, srcBits, dst,
           dstBits);
  uint8_t smask = 0x01 << srcBits;
  uint8_t dmask = 0x01 << dstBits;
  uint8_t _src = src & smask;
  uint8_t _dst;
  if (_src != 0) {
    _dst = dst | dmask;  // set bit
  } else {
    _dst = dst & ~(dmask);  // clear bit
  }
  return _dst;
}

// Rotate 8-bit data
// 0x12-->0x48
uint8_t ssd1306_rotate_byte(uint8_t ch1) {
  uint8_t ch2 = 0;
  for (int j = 0; j < 8; j++) {
    ch2 = (ch2 << 1) + (ch1 & 0x01);
    ch1 = ch1 >> 1;
  }
  return ch2;
}

void ssd1306_fadeout(ssd1306_t *ssd) {
  uint8_t image[1];
  for (int page = 0; page < ssd->pages; page++) {
    image[0] = 0xFF;
    for (int line = 0; line < 8; line++) {
      if (ssd->flip) {
        image[0] = image[0] >> 1;
      } else {
        image[0] = image[0] << 1;
      }
      for (int seg = 0; seg < 128; seg++) {
        i2c_display_image(ssd, page, seg, image, 1);
        ssd->screen_pages[page]._segs[seg] = image[0];
      }
    }
  }
}

void ssd1306_dump_page(ssd1306_t *ssd, int page, int seg) {
  ESP_LOGI(TAG, "ssd->screen_pages[%d]._segs[%d]=%02x", page, seg,
           ssd->screen_pages[page]._segs[seg]);
}
