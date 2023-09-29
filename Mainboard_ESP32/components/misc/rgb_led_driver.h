// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#include "led_driver.h"

/*!
 * \file rgb_led_driver.h
 * \brief RGB LED driver for ESP32
 */

/*!
 * \brief RGB LED color enum
 */
typedef enum {
  NONE = 0b000,
  RED = 0b001,
  GREEN = 0b010,
  BLUE = 0b100,
  YELLOW = RED | GREEN,
  CYAN = GREEN | BLUE,
  MAGENTA = RED | BLUE,
  WHITE = RED | GREEN | BLUE
} rgb_led_color_t;

/*!
 * \brief RGB LED driver table index enum
 */
typedef enum {
  RED_INDEX = 0,
  GREEN_INDEX = 1,
  BLUE_INDEX = 2,
  MAX_COLOR_INDEX = 3
} rgb_led_driver_table_t;

/*!
 * \brief RGB LED driver struct for ESP32
 */
typedef struct {
  led_driver_t led_drv[MAX_COLOR_INDEX];
} rgb_led_driver_t;

/*!
 * \brief Initialize RGB LED driver
 * \param rgb_led_drv Pointer to rgb_led_driver_t struct
 * \return ESP_OK on success, ESP_FAIL otherwise
 */
esp_err_t rgb_led_driver_init(rgb_led_driver_t *rgb_led_drv);
