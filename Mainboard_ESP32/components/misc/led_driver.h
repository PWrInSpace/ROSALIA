// Copyright 2023 PWr in Space, Krzysztof Gliwiński
#pragma once

#include <stdint.h>

#include "driver/ledc.h"
#include "esp_err.h"
#include "esp_log.h"

/*!
 * \file led_driver.h
 * \brief LED driver for ESP32
 */

#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_MODE LEDC_LOW_SPEED_MODE
#define LEDC_DUTY_RES LEDC_TIMER_13_BIT  // Set duty resolution to 13 bits
#define LEDC_FREQUENCY (5000)  // Frequency in Hertz. Set frequency at 5 kHz
#define MAX_LED_DUTY 8190

#define LED_DRIVER_TAG "LED Driver"

/*!
    \brief Intended for LEDs that are positive voltage driven!
*/
typedef struct {
  uint8_t led_gpio_num;
  uint8_t ledc_channel_num;
  uint8_t ledc_timer_num;
  uint16_t duty;
} led_driver_t;

/*!
 * \brief Initialize LED driver
 * \param led_drv Pointer to led_driver_t struct
 * \return ESP_OK on success, ESP_FAIL otherwise
 */
esp_err_t led_driver_init(led_driver_t *led_drv);

/*!
 * \brief Update duty cycle of LED
 * \param led_drv Pointer to led_driver_t struct
 * \param duty Duty cycle in range 0-8190
 * \return ESP_OK on success, ESP_FAIL otherwise
 */
esp_err_t led_driver_update_duty_cycle(led_driver_t *led_drv, uint16_t duty);
