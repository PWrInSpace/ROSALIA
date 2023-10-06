// Copyright 2022 PWrInSpace, Krzysztof Gliwiński

#pragma once
#include <stdbool.h>
#include <stdint.h>

#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_adc/adc_oneshot.h"
#include "soc/adc_channel.h"

/*!
  \file voltage_measure.h contains structs and functions to handle
        ESP32's ADC1 only!
*/

#define READ_ERROR_RETURN_VAL 0xFFFF
#define VOLTAGE_READ_ERROR_RETURN_VAL -1.0f

typedef struct {
  float adc_cal;
  uint8_t adc_chan;
  adc_oneshot_unit_init_cfg_t oneshot_unit_cfg;
} voltage_measure_config_t;

/*!
  \brief Init for a voltage measure.
  \param v_mes - pointer to voltage_measure_config_t struct
  \param adc_chan - specific channel of ADC1
  \param adc_cal - calibration value to be configured.
                  voltage = rawRead * adc_cal
*/
esp_err_t voltage_measure_init(voltage_measure_config_t* v_mes,
                               uint8_t adc_chan, float adc_cal);

/*!
 * \brief Read raw value from ADC
 * \param v_mes - pointer to voltage_measure_config_t struct
 */
int voltage_measure_read_raw(voltage_measure_config_t* v_mes);

/*!
 * \brief Read voltage from ADC
 * \param v_mes - pointer to voltage_measure_config_t struct
 */
float voltage_measure_read_voltage(voltage_measure_config_t* v_mes);
