// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#pragma once

#include "buzzer_driver.h"
#include "rgb_led_driver.h"

#define RGB_LED_FREQ_HZ 5000
#define RGB_LED_DUTY_RES LEDC_TIMER_13_BIT
#define RGB_LED_TIMER LEDC_TIMER_0
#define RGB_LED_MODE LEDC_LOW_SPEED_MODE
#define RGB_LED_MAX_DUTY 8191  // 2**13 - 1

#define BUZZER_FREQ_HZ 4000
#define BUZZER_DUTY_RES LEDC_TIMER_13_BIT
#define BUZZER_TIMER LEDC_TIMER_1
#define BUZZER_MODE LEDC_LOW_SPEED_MODE
#define BUZZER_MAX_DUTY 8191  // 2**13 - 1

#define MCU_RGB_LED_DRIVER_DEFAULT_CONFIG()                          \
  {                                                                  \
    .led_drv = {[RED_INDEX] = {.ledc_mode = RGB_LED_MODE,            \
                               .led_gpio_num = CONFIG_LED_K_R,       \
                               .ledc_channel_num = LEDC_CHANNEL_0,   \
                               .ledc_timer_num = RGB_LED_TIMER,      \
                               .duty = 0,                            \
                               .max_duty = RGB_LED_MAX_DUTY,         \
                               .toggle = LED_OFF,                    \
                               .inverted = true},                    \
                [GREEN_INDEX] = {.ledc_mode = RGB_LED_MODE,          \
                                 .led_gpio_num = CONFIG_LED_K_G,     \
                                 .ledc_channel_num = LEDC_CHANNEL_1, \
                                 .ledc_timer_num = RGB_LED_TIMER,    \
                                 .duty = 0,                          \
                                 .max_duty = RGB_LED_MAX_DUTY,       \
                                 .toggle = LED_OFF,                  \
                                 .inverted = true},                  \
                [BLUE_INDEX] = {.ledc_mode = RGB_LED_MODE,           \
                                .led_gpio_num = CONFIG_LED_K_B,      \
                                .ledc_channel_num = LEDC_CHANNEL_2,  \
                                .ledc_timer_num = RGB_LED_TIMER,     \
                                .duty = 0,                           \
                                .max_duty = RGB_LED_MAX_DUTY,        \
                                .toggle = LED_OFF,                   \
                                .inverted = true}},                  \
    .max_duty = RGB_LED_MAX_DUTY                                     \
  }

#define MCU_BUZZER_DRIVER_DEFAULT_CONFIG()                              \
  {                                                                     \
    .ledc_mode = BUZZER_MODE, .gpio_num = CONFIG_BUZZER,                \
    .ledc_channel_num = LEDC_CHANNEL_3, .ledc_timer_num = BUZZER_TIMER, \
    .duty = 0, .max_duty = BUZZER_MAX_DUTY,                             \
  }

/*!
 * \brief Initialize misc drivers
 * \param buzzer buzzer driver
 * \param rgb_led rgb led driver
 */
esp_err_t misc_init(buzzer_driver_t *buzzer, rgb_led_driver_t *rgb_led);
