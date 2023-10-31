// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#pragma once

#include "buzzer_driver.h"
#include "driver/twai.h"
#include "esp_log.h"
#include "lora.h"
#include "lora_esp32_config.h"
#include "mcu_adc_config.h"
#include "mcu_i2c_config.h"
#include "mcu_spi_config.h"
#include "mcu_twai_config.h"
#include "rgb_led_driver.h"
#include "sdkconfig.h"
#include "ssd1306_esp32_config.h"

#define LEDC_FREQ_HZ 5000
#define LEDC_DUTY_RES LEDC_TIMER_13_BIT
#define LEDC_HS_TIMER LEDC_TIMER_0
#define LEDC_HS_MODE LEDC_LOW_SPEED_MODE
#define MAX_DUTY 8191  // 2**13 - 1

#define MCU_SPI_DEFAULT_CONFIG         \
  = {                                  \
      .host = HSPI_HOST,               \
      .mosi = CONFIG_SPI_MOSI,         \
      .miso = CONFIG_SPI_MISO,         \
      .sclk = CONFIG_SPI_SCLK,         \
      .cs = CONFIG_SPI_CS,             \
      .clk_speed = SPI_MASTER_FREQ_8M, \
      .spi_init_flag = false,          \
  }

#define MCU_I2C_DEFAULT_CONFIG()                                     \
  {                                                                  \
    .port = I2C_NUM_0, .sda = CONFIG_I2C_SDA, .scl = CONFIG_I2C_SCL, \
    .clk_speed = I2C_MASTER_FREQ_HZ, .i2c_init_flag = false,         \
  }

#define MCU_TWAI_DEFAULT_CONFIG()                                     \
  {                                                                   \
    .tx_gpio_num = CONFIG_UART_1_RX, .rx_gpio_num = CONFIG_UART_1_TX, \
    .mode = TWAI_MODE_NORMAL                                          \
  }

#define MCU_VOLTAGE_MEASURE_DEFAULT_CONFIG()               \
  {                                                        \
    .adc_cal = {1.0f, 1.0f, 1.0f},                         \
    .adc_chan = {CAN_CHANNEL, VBAT_CHANNEL, ADJV_CHANNEL}, \
    .adc_chan_num = MAX_CHANNEL_INDEX,                     \
    .oneshot_chan_cfg =                                    \
        {                                                  \
            .bitwidth = ADC_BITWIDTH_12,                   \
            .atten = ADC_ATTEN_DB_11,                      \
        },                                                 \
    .oneshot_unit_cfg = {                                  \
      .unit_id = ADC_UNIT_1,                               \
    }                                                      \
  }

#define MCU_SSD1306_DEFAULT_CONFIG()                              \
  {                                                               \
    ._i2c_master_write_byte = _ssd1306_i2c_master_write_byte,     \
    ._i2c_master_write = _ssd1306_i2c_master_write,               \
    ._i2c_master_start = _ssd1306_i2c_master_start,               \
    ._i2c_master_stop = _ssd1306_i2c_master_stop,                 \
    ._i2c_master_cmd_begin = _ssd1306_i2c_master_cmd_begin,       \
    ._i2c_cmd_link_create = _ssd1306_i2c_cmd_link_create,         \
    ._i2c_cmd_link_delete = _ssd1306_i2c_cmd_link_delete,         \
    ._delay = _ssd1306_delay, ._log = _ssd1306_log, .width = 128, \
    .height = 64, .pages = 1, .i2c_address = 0x3c,                \
  }

#define MCU_RGB_LED_DRIVER_DEFAULT_CONFIG()                          \
  {                                                                  \
    .led_drv = {[RED_INDEX] = {.ledc_mode = LEDC_HS_MODE,            \
                               .led_gpio_num = CONFIG_LED_K_R,       \
                               .ledc_channel_num = LEDC_CHANNEL_0,   \
                               .ledc_timer_num = LEDC_HS_TIMER,      \
                               .duty = 0,                            \
                               .max_duty = MAX_DUTY,                 \
                               .toggle = LED_OFF,                    \
                               .inverted = true},                    \
                [GREEN_INDEX] = {.ledc_mode = LEDC_HS_MODE,          \
                                 .led_gpio_num = CONFIG_LED_K_G,     \
                                 .ledc_channel_num = LEDC_CHANNEL_1, \
                                 .ledc_timer_num = LEDC_HS_TIMER,    \
                                 .duty = 0,                          \
                                 .max_duty = MAX_DUTY,               \
                                 .toggle = LED_OFF,                  \
                                 .inverted = true},                  \
                [BLUE_INDEX] = {.ledc_mode = LEDC_HS_MODE,           \
                                .led_gpio_num = CONFIG_LED_K_B,      \
                                .ledc_channel_num = LEDC_CHANNEL_2,  \
                                .ledc_timer_num = LEDC_HS_TIMER,     \
                                .duty = 0,                           \
                                .max_duty = MAX_DUTY,                \
                                .toggle = LED_OFF,                   \
                                .inverted = true}},                  \
    .max_duty = MAX_DUTY                                             \
  }

typedef struct {
  mcu_spi_config_t spi;
  mcu_i2c_config_t i2c;
  mcu_twai_config_t twai;
  voltage_measure_config_t voltage_measure;
  lora_struct_t lora;
  ssd1306_t oled_display;
  rgb_led_driver_t rgb_led;
} ROSALIA_devices_t;

/*!
 * \brief Initiates all devices
 * \param devices Devices configuration
 * \return ESP_OK on success, ESP_FAIL otherwise
 * \note This function will initiate all devices.
 */
esp_err_t devices_init(ROSALIA_devices_t *devices);
