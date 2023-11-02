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

#define MCU_TWAI_DEFAULT_CONFIG()                                     \
  {                                                                   \
    .tx_gpio_num = CONFIG_UART_1_RX, .rx_gpio_num = CONFIG_UART_1_TX, \
    .mode = TWAI_MODE_NORMAL                                          \
  }

#define MCU_SPI_DEFAULT_CONFIG()                   \
  {                                                \
    .host_id = SPI2_HOST,                          \
    .bus_config = {.miso_io_num = CONFIG_SPI_MISO, \
                   .mosi_io_num = CONFIG_SPI_MOSI, \
                   .sclk_io_num = CONFIG_SPI_SCK,  \
                   .quadwp_io_num = -1,            \
                   .quadhd_io_num = -1,            \
                   .max_transfer_sz = 4000},          \
    .dev_config = {.clock_speed_hz = 9000000,      \
                   .mode = 0,                      \
                   .spics_io_num = -1,             \
                   .queue_size = 1,                \
                   .flags = 0,                     \
                   .pre_cb = NULL},                \
    .spi_init_flag = false,                        \
  }

#define MCU_LORA_DEFAULT_CONFIG()                                       \
  {                                                                     \
    ._spi_transmit = _lora_SPI_transmit, ._delay = _lora_delay,         \
    ._gpio_set_level = _lora_GPIO_set_level, .log = _lora_log,          \
    .rst_gpio_num = CONFIG_LORA_RS, .cs_gpio_num = CONFIG_LORA_CS,      \
    .d0_gpio_num = CONFIG_LORA_D0, .implicit_header = 0, .frequency = 0 \
  }

#define MCU_I2C_DEFAULT_CONFIG()                                     \
  {                                                                  \
    .port = I2C_NUM_0, .sda = CONFIG_I2C_SDA, .scl = CONFIG_I2C_SCL, \
    .clk_speed = I2C_MASTER_FREQ_HZ, .i2c_init_flag = false,         \
  }

#define MCU_VOLTAGE_MEASURE_DEFAULT_CONFIG()               \
  {                                                        \
    .adc_cal = {1.0f, 5.742f, 5.180f},                 \
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

typedef struct {
  mcu_spi_config_t spi;
  mcu_i2c_config_t i2c;
  mcu_twai_config_t twai;
  voltage_measure_config_t voltage_measure;
  lora_struct_t lora;
  ssd1306_t oled_display;
  rgb_led_driver_t rgb_led;
  buzzer_driver_t buzzer;
} ROSALIA_devices_t;

/*!
 * \brief Initiates all devices
 * \param devices Devices configuration
 * \return ESP_OK on success, ESP_FAIL otherwise
 * \note This function will initiate all devices.
 */
esp_err_t devices_init(ROSALIA_devices_t *devices);
