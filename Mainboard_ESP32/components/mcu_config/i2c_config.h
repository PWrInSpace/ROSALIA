// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#include "driver/i2c.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"

typedef struct {
  i2c_port_t port;
  i2c_cmd_handle_t *cmd;
  gpio_num_t sda;
  gpio_num_t scl;
  uint32_t clk_speed;
  uint32_t timeout;
  bool i2c_init_flag;
} i2c_config_t;

/*!
 * \brief Initiates the I2C bus
 * \param i2c I2C configuration
 * \return ESP_OK on success, ESP_FAIL otherwise
 * \note This function will initiate I2C peripheral.
 */
esp_err_t i2c_init(i2c_config_t *i2c);
