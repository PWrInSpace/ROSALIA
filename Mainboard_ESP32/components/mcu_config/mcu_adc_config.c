// Copyright 2023 PWrInSpace, Krzysztof Gliwiński

#include "mcu_adc_config.h"

#define ADC_CONFIG_TAG "ADC_CONFIG"

esp_err_t voltage_measure_init(voltage_measure_config_t* v_mes) {
  if (v_mes->adc_chan_num > MAX_ADC_CHANNELS) {
    ESP_LOGE(ADC_CONFIG_TAG, "Too many ADC channels to configure!");
    return ESP_FAIL;
  }
  esp_err_t res = ESP_OK;

  res |= adc_oneshot_new_unit(&v_mes->oneshot_unit_cfg,
                              &v_mes->oneshot_unit_handle);

  for (uint8_t i = 0; i < v_mes->adc_chan_num; i++) {
    res |= adc_oneshot_config_channel(v_mes->oneshot_unit_handle,
                                      v_mes->adc_chan[i],
                                      &v_mes->oneshot_chan_cfg);
  }

  return res;
}

int voltage_measure_read_raw(voltage_measure_config_t* v_mes,
                             uint8_t adc_chan) {
  int vRaw;
  if (adc_chan > v_mes->adc_chan_num) {
    ESP_LOGE(ADC_CONFIG_TAG, "ADC channel out of range!");
    return READ_ERROR_RETURN_VAL;
  }
  if (adc_oneshot_read(v_mes->oneshot_unit_handle, v_mes->adc_chan[adc_chan],
                       &vRaw) == ESP_FAIL) {
    return READ_ERROR_RETURN_VAL;
  }
  return vRaw;
}

float voltage_measure_read_voltage(voltage_measure_config_t* v_mes,
                                   uint8_t adc_chan) {
  int vRaw = voltage_measure_read_raw(v_mes, adc_chan);
  if (vRaw == READ_ERROR_RETURN_VAL) {
    return VOLTAGE_READ_ERROR_RETURN_VAL;
  }
  return v_mes->adc_cal[adc_chan] * (float)vRaw / 4095.0f * 3.3f;
}
