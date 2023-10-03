// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#include <string.h>

#include "driver/i2c.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ssd1306.h"

#define tag "SSD1306"

#if CONFIG_I2C_PORT_0
#define I2C_NUM I2C_NUM_0
#elif CONFIG_I2C_PORT_1
#define I2C_NUM I2C_NUM_1
#else
#define I2C_NUM I2C_NUM_0  // if spi is selected
#endif

// TODO(Glibus): move to sdkconfig (kconfig.projbuild)
#define I2C_MASTER_FREQ_HZ \
  400000 /*!< I2C clock of SSD1306 can run at 400 kHz max. */

#define CONFIG_OFFSETX 0

// TODO(Glibus): i2c init moved to another lib
//  void i2c_master_init(ssd1306_t* ssd, int16_t sda, int16_t scl, int16_t
//  reset) {
//    i2c_config_t i2c_config = {.mode = I2C_MODE_MASTER,
//                               .sda_io_num = sda,
//                               .scl_io_num = scl,
//                               .sda_pullup_en = GPIO_PULLUP_ENABLE,
//                               .scl_pullup_en = GPIO_PULLUP_ENABLE,
//                               .master.clk_speed = I2C_MASTER_FREQ_HZ};
//    ESP_ERROR_CHECK(i2c_param_config( &i2c_config));
//    ESP_ERROR_CHECK(i2c_driver_install( I2C_MODE_MASTER, 0, 0, 0));

// //   ssd->i2c_address = I2CAddress;
//   ssd->flip = false;
// }

void i2c_init(ssd1306_t* ssd, uint8_t width, uint8_t height) {
  ssd->width = width;
  ssd->height = height;
  ssd->pages = 8;
  if (ssd->height == 32) {
    ssd->pages = 4;
  }

  ssd1306_i2c_cmd_handle_t cmd = ssd->_i2c_cmd_link_create();

  ssd->_i2c_master_start(cmd);
  ssd->_i2c_master_write_byte(
      (ssd->i2c_address << 1) | ssd->i2c_master_write_flag, true);
  ssd->_i2c_master_write_byte(OLED_CONTROL_BYTE_CMD_STREAM, true);
  ssd->_i2c_master_write_byte(OLED_CMD_DISPLAY_OFF, true);    // AE
  ssd->_i2c_master_write_byte(OLED_CMD_SET_MUX_RATIO, true);  // A8
  if (ssd->height == 64) {
    ssd->_i2c_master_write_byte(0x3F, true);
  }

  if (ssd->height == 32) {
    ssd->_i2c_master_write_byte(0x1F, true);
  }
  ssd->_i2c_master_write_byte(OLED_CMD_SET_DISPLAY_OFFSET, true);  // D3
  ssd->_i2c_master_write_byte(0x00, true);
  // ssd->_i2c_master_write_byte( OLED_CONTROL_BYTE_DATA_STREAM, true);  // 40
  ssd->_i2c_master_write_byte(OLED_CMD_SET_DISPLAY_START_LINE, true);  // 40
  // ssd->_i2c_master_write_byte( OLED_CMD_SET_SEGMENT_REMAP, true);
  // // A1
  if (ssd->flip) {
    ssd->_i2c_master_write_byte(OLED_CMD_SET_SEGMENT_REMAP_0, true);  // A0
  } else {
    ssd->_i2c_master_write_byte(OLED_CMD_SET_SEGMENT_REMAP_1, true);  // A1
  }
  ssd->_i2c_master_write_byte(OLED_CMD_SET_COM_SCAN_MODE, true);    // C8
  ssd->_i2c_master_write_byte(OLED_CMD_SET_DISPLAY_CLK_DIV, true);  // D5
  ssd->_i2c_master_write_byte(0x80, true);
  ssd->_i2c_master_write_byte(OLED_CMD_SET_COM_PIN_MAP, true);  // DA
  if (ssd->height == 64) {
    ssd->_i2c_master_write_byte(0x12, true);
  }
  if (ssd->height == 32) {
    ssd->_i2c_master_write_byte(0x02, true);
  }
  ssd->_i2c_master_write_byte(OLED_CMD_SET_CONTRAST, true);  // 81
  ssd->_i2c_master_write_byte(0xFF, true);
  ssd->_i2c_master_write_byte(OLED_CMD_DISPLAY_RAM, true);        // A4
  ssd->_i2c_master_write_byte(OLED_CMD_SET_VCOMH_DESELCT, true);  // DB
  ssd->_i2c_master_write_byte(0x40, true);
  ssd->_i2c_master_write_byte(OLED_CMD_SET_MEMORY_ADDR_MODE, true);  // 20
  // ssd->_i2c_master_write_byte( OLED_CMD_SET_HORI_ADDR_MODE, true);  // 00
  ssd->_i2c_master_write_byte(OLED_CMD_SET_PAGE_ADDR_MODE, true);  // 02
  // Set Lower Column Start Address for Page Addressing Mode
  ssd->_i2c_master_write_byte(0x00, true);
  // Set Higher Column Start Address for Page Addressing Mode
  ssd->_i2c_master_write_byte(0x10, true);
  ssd->_i2c_master_write_byte(OLED_CMD_SET_CHARGE_PUMP, true);  // 8D
  ssd->_i2c_master_write_byte(0x14, true);
  ssd->_i2c_master_write_byte(OLED_CMD_DEACTIVE_SCROLL, true);  // 2E
  ssd->_i2c_master_write_byte(OLED_CMD_DISPLAY_NORMAL, true);   // A6
  ssd->_i2c_master_write_byte(OLED_CMD_DISPLAY_ON, true);       // AF

  ssd->_i2c_master_stop();

  esp_err_t espRc = ssd->_i2c_master_cmd_begin(cmd, 10);
  if (espRc == ESP_OK) {
    ESP_LOGI(tag, "OLED configured successfully");
  } else {
    ESP_LOGE(tag, "OLED configuration failed. code: 0x%.2X", espRc);
  }
  ssd->_i2c_cmd_link_delete();
}

void i2c_display_image(ssd1306_t* ssd, int page, int seg, uint8_t* images,
                       uint8_t width) {
  ssd1306_i2c_cmd_handle_t cmd = cmd;

  if (page >= ssd->pages) {
    return;
  }
  if (seg >= ssd->width) {
    return;
  }

  int _seg = seg + CONFIG_OFFSETX;
  uint8_t columLow = _seg & 0x0F;
  uint8_t columHigh = (_seg >> 4) & 0x0F;

  int screen_pages = page;
  if (ssd->flip) {
    screen_pages = (ssd->pages - page) - 1;
  }

  ssd->_i2c_cmd_link_create();
  ssd->_i2c_master_start(cmd);
  ssd->_i2c_master_write_byte(
      (ssd->i2c_address << 1) | ssd->i2c_master_write_flag, true);

  ssd->_i2c_master_write_byte(OLED_CONTROL_BYTE_CMD_STREAM, true);
  // Set Lower Column Start Address for Page Addressing Mode
  ssd->_i2c_master_write_byte((0x00 + columLow), true);
  // Set Higher Column Start Address for Page Addressing Mode
  ssd->_i2c_master_write_byte((0x10 + columHigh), true);
  // Set Page Start Address for Page Addressing Mode
  ssd->_i2c_master_write_byte(0xB0 | screen_pages, true);

  ssd->_i2c_master_stop();
  ssd->_i2c_master_cmd_begin(cmd, 10);
  ssd->_i2c_cmd_link_delete();

  ssd->_i2c_cmd_link_create();
  ssd->_i2c_master_start(cmd);
  ssd->_i2c_master_write_byte(
      (ssd->i2c_address << 1) | ssd->i2c_master_write_flag, true);

  ssd->_i2c_master_write_byte(OLED_CONTROL_BYTE_DATA_STREAM, true);
  ssd->_i2c_master_write(images, width, true);

  ssd->_i2c_master_stop();
  ssd->_i2c_master_cmd_begin(cmd, 10);
  ssd->_i2c_cmd_link_delete();
}

void i2c_contrast(ssd1306_t* ssd, int contrast) {
  ssd1306_i2c_cmd_handle_t cmd = cmd;
  int _contrast = contrast;
  if (contrast < 0x0) {
    _contrast = 0;
  }
  if (contrast > 0xFF) {
    _contrast = 0xFF;
  }

  ssd->_i2c_cmd_link_create();
  ssd->_i2c_master_start(cmd);
  ssd->_i2c_master_write_byte(
      (ssd->i2c_address << 1) | ssd->i2c_master_write_flag, true);
  ssd->_i2c_master_write_byte(OLED_CONTROL_BYTE_CMD_STREAM, true);
  ssd->_i2c_master_write_byte(OLED_CMD_SET_CONTRAST, true);  // 81
  ssd->_i2c_master_write_byte(_contrast, true);
  ssd->_i2c_master_stop();
  ssd->_i2c_master_cmd_begin(cmd, 10);
  ssd->_i2c_cmd_link_delete();
}

void i2c_hardware_scroll(ssd1306_t* ssd, ssd1306_scroll_type_t scroll) {
  esp_err_t espRc;

  ssd1306_i2c_cmd_handle_t cmd = cmd = ssd->_i2c_cmd_link_create();
  ssd->_i2c_master_start(cmd);

  ssd->_i2c_master_write_byte(
      (ssd->i2c_address << 1) | ssd->i2c_master_write_flag, true);
  ssd->_i2c_master_write_byte(OLED_CONTROL_BYTE_CMD_STREAM, true);

  if (scroll == SCROLL_RIGHT) {
    ssd->_i2c_master_write_byte(OLED_CMD_HORIZONTAL_RIGHT, true);  // 26
    ssd->_i2c_master_write_byte(0x00, true);                       // Dummy byte
    ssd->_i2c_master_write_byte(0x00, true);  // Define start page address
    ssd->_i2c_master_write_byte(0x07, true);  // Frame frequency
    ssd->_i2c_master_write_byte(0x07, true);  // Define end page address
    ssd->_i2c_master_write_byte(0x00, true);  //
    ssd->_i2c_master_write_byte(0xFF, true);  //
    ssd->_i2c_master_write_byte(OLED_CMD_ACTIVE_SCROLL, true);  // 2F
  }

  if (scroll == SCROLL_LEFT) {
    ssd->_i2c_master_write_byte(OLED_CMD_HORIZONTAL_LEFT, true);  // 27
    ssd->_i2c_master_write_byte(0x00, true);                      // Dummy byte
    ssd->_i2c_master_write_byte(0x00, true);  // Define start page address
    ssd->_i2c_master_write_byte(0x07, true);  // Frame frequency
    ssd->_i2c_master_write_byte(0x07, true);  // Define end page address
    ssd->_i2c_master_write_byte(0x00, true);  //
    ssd->_i2c_master_write_byte(0xFF, true);  //
    ssd->_i2c_master_write_byte(OLED_CMD_ACTIVE_SCROLL, true);  // 2F
  }

  if (scroll == SCROLL_DOWN) {
    ssd->_i2c_master_write_byte(OLED_CMD_CONTINUOUS_SCROLL, true);  // 29
    ssd->_i2c_master_write_byte(0x00, true);  // Dummy byte
    ssd->_i2c_master_write_byte(0x00, true);  // Define start page address
    ssd->_i2c_master_write_byte(0x07, true);  // Frame frequency
    // ssd->_i2c_master_write_byte( 0x01, true); // Define end page address
    ssd->_i2c_master_write_byte(0x00, true);  // Define end page address
    ssd->_i2c_master_write_byte(0x3F, true);  // Vertical scrolling offset

    ssd->_i2c_master_write_byte(OLED_CMD_VERTICAL, true);  // A3
    ssd->_i2c_master_write_byte(0x00, true);
    if (ssd->height == 64) {
      // ssd->_i2c_master_write_byte( 0x7F, true);
      ssd->_i2c_master_write_byte(0x40, true);
    }
    if (ssd->height == 32) {
      ssd->_i2c_master_write_byte(0x20, true);
    }
    ssd->_i2c_master_write_byte(OLED_CMD_ACTIVE_SCROLL, true);  // 2F
  }

  if (scroll == SCROLL_UP) {
    ssd->_i2c_master_write_byte(OLED_CMD_CONTINUOUS_SCROLL, true);  // 29
    ssd->_i2c_master_write_byte(0x00, true);  // Dummy byte
    ssd->_i2c_master_write_byte(0x00, true);  // Define start page address
    ssd->_i2c_master_write_byte(0x07, true);  // Frame frequency
    // ssd->_i2c_master_write_byte( 0x01, true); // Define end page address
    ssd->_i2c_master_write_byte(0x00, true);  // Define end page address
    ssd->_i2c_master_write_byte(0x01, true);  // Vertical scrolling offset

    ssd->_i2c_master_write_byte(OLED_CMD_VERTICAL, true);  // A3
    ssd->_i2c_master_write_byte(0x00, true);
    if (ssd->height == 64) {
      // ssd->_i2c_master_write_byte( 0x7F, true);
      ssd->_i2c_master_write_byte(0x40, true);
    }
    if (ssd->height == 32) {
      ssd->_i2c_master_write_byte(0x20, true);
    }
    ssd->_i2c_master_write_byte(OLED_CMD_ACTIVE_SCROLL, true);  // 2F
  }

  if (scroll == SCROLL_STOP) {
    ssd->_i2c_master_write_byte(OLED_CMD_DEACTIVE_SCROLL, true);  // 2E
  }

  ssd->_i2c_master_stop();
  espRc = ssd->_i2c_master_cmd_begin(cmd, 10);
  if (espRc == ESP_OK) {
    ESP_LOGD(tag, "Scroll command succeeded");
  } else {
    ESP_LOGE(tag, "Scroll command failed. code: 0x%.2X", espRc);
  }

  ssd->_i2c_cmd_link_delete();
}
