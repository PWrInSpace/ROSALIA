// Copyright 2023 PWr in Space, Krzysztof Gliwiński
#pragma once

#include "rgb_led_driver.h"

/*!
 * \file user_interface.h
 * \brief User interface app header file
 * Contains task for rgb LED, status LED, buzzer and button handling
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
