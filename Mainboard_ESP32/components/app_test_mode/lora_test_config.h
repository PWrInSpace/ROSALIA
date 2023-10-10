// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#include "config.h"
#include "driver/uart.h"
#include "esp_console.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_vfs_dev.h"
#include "esp_vfs_fat.h"
#include "freertos/FreeRTOS.h"
#include "lora_esp32_config.h"
#include "lora.h"
#include "sdkconfig.h"

#define TAG "MAIN"
// TODO(Glibus): Change the GPIO nums to another struct apply in
// lora_esp32_config files
lora_struct_t lora = {._spi_transmit = _lora_SPI_transmit,
                      ._delay = _lora_delay,
                      ._gpio_set_level = _lora_GPIO_set_level,
                      .log = _lora_log,
                      .rst_gpio_num = CONFIG_LORA_RS,
                      .cs_gpio_num = CONFIG_LORA_CS,
                      .d0_gpio_num = CONFIG_LORA_D0,
                      .implicit_header = 0,
                      .frequency = 0};

/*!
 * \brief Initiates lora and uart
 */
void lora_test_init();

/*!
 * \brief LoRa sender task - sends data from UART to LoRa
 * \param[in] param Unused - FreeRTOS task parameter
 */
void task_lora_tx(void *param);

/*!
 * \brief LoRa receiver task - receives data from LoRa and sends it to UART
 * \param[in] param Unused - FreeRTOS task parameter
 */
void task_lora_rx(void *param);
