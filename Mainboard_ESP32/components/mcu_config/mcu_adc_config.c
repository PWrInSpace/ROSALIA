// Copyright 2022 PWrInSpace, Krzysztof Gliwiński

#include "mcu_adc_config.h"

#define ONESHOT_UNIT_UNDEFINED 0
#define ONESHOT_UNIT_DEFINED 1

/// \brief Standard config, enables ADC on the whole 0 - 3.3V
/// range on ESP32 ADC1
static adc_oneshot_chan_cfg_t configAdc1 = {.bitwidth = ADC_BITWIDTH_DEFAULT,
                                            .atten = ADC_ATTEN_DB_11};

static adc_oneshot_unit_init_cfg_t initConfigAdc1 = {
    .unit_id = ADC_UNIT_1,
};

static adc_oneshot_unit_handle_t handleAdc1;

esp_err_t voltage_measure_init(voltage_measure_config_t* v_mes, uint8_t adc_chan,
                             float adc_cal) {
  esp_err_t res = ESP_OK;
  static bool oneshotUnitInitiated = false;
  v_mes->adc_cal = adc_cal;
  v_mes->adc_chan = adc_chan;
  if (oneshotUnitInitiated == ONESHOT_UNIT_UNDEFINED) {
    res |= adc_oneshot_new_unit(&initConfigAdc1, &handleAdc1);
    oneshotUnitInitiated = ONESHOT_UNIT_DEFINED;
  }
  res |= adc_oneshot_config_channel(handleAdc1, adc_chan, &configAdc1);

  return res;
}

int voltage_measure_read_raw(voltage_measure_config_t* v_mes) {
  int vRaw;
  if (adc_oneshot_read(handleAdc1, v_mes->adc_chan, &vRaw) == ESP_FAIL) {
    return READ_ERROR_RETURN_VAL;
  }
  return vRaw;
}

float voltage_measure_read_voltage(voltage_measure_config_t* v_mes) {
  int vRaw = voltage_measure_read_raw(v_mes);
  if (vRaw == READ_ERROR_RETURN_VAL) {
    return VOLTAGE_READ_ERROR_RETURN_VAL;
  }
  return v_mes->adc_cal * (float)vRaw / 4095.0f * 3.3f;
}
