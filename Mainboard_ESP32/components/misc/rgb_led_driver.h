// Copyright 2023 PWr in Space, Krzysztof Gliwiński
#pragma once

#include "led_driver.h"

/*!
 * \file rgb_led_driver.h
 * \brief RGB LED driver for ESP32
 */

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
 * \param led_drv LED driver struct
 * \param max_duty Maximum duty cycle: \note forces all colors to have the same
 */
typedef struct {
  led_driver_t led_drv[MAX_COLOR_INDEX];
  uint16_t max_duty;
} rgb_led_driver_t;

/*!
 * \brief Initialize RGB LED driver
 * \param rgb_led_drv Pointer to rgb_led_driver_t struct
 * \param ledc_duty_res LEDC duty resolution
 * \param ledc_freq LEDC frequency
 * \return ESP_OK on success, ESP_FAIL otherwise
 */
esp_err_t rgb_led_driver_init(rgb_led_driver_t *rgb_led_drv,
                              ledc_timer_bit_t ledc_duty_res,
                              uint32_t ledc_freq);

/*!
 * \brief Update duty cycle of RGB LED
 * \param rgb_led_drv Pointer to rgb_led_driver_t struct
 * \param duty Duty cycle in range 0-max_duty
 * \return ESP_OK on success, ESP_FAIL otherwise
 */
esp_err_t rgb_led_update_duty_cycle(rgb_led_driver_t *rgb_led_drv,
                                    uint16_t duty[MAX_COLOR_INDEX]);
