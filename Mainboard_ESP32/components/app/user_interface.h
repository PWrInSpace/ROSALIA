// Copyright 2023 PWr in Space, Krzysztof Gliwiński
#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rgb_led_driver.h"
#include "ssd1306_esp32_config.h"
#include "sdkconfig.h"

/*!
 * \file user_interface.h
 * \brief User interface app header file
 * Contains task for rgb LED, status LED, buzzer and button handling
 */

#define LEDC_FREQ_HZ 5000
#define LEDC_DUTY_RES LEDC_TIMER_13_BIT
#define LEDC_HS_TIMER LEDC_TIMER_0
#define LEDC_HS_MODE LEDC_LOW_SPEED_MODE
#define MAX_DUTY 8192  // 2**13

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


void user_interface_task(void* arg);

void init_user_interface_task(void* arg);
