// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#include "usb_msc_api.h"

#define USB_API_TAG "USB_MSC_API"

esp_err_t usb_msc_init(usb_msc_config_t* config) {
  esp_err_t ret = ESP_OK;
  ret |= tinyusb_msc_storage_init_sdmmc(&config->sd_config);
  if (ret != ESP_OK) {
    ESP_LOGE(USB_API_TAG, "MSC Storage sdmmc init failed");
    return ret;
  }
  ret |= usb_msc_mount(config);
  if (ret != ESP_OK) {
    ESP_LOGE(USB_API_TAG, "MSC Storage mount failed");
    return ret;
  }
  ESP_LOGI(USB_API_TAG, "MSC initialization");
  ret |= tinyusb_driver_install(&config->tusb_cfg);
  return ret;
}

void usb_msc_deinit(usb_msc_config_t* config) {
  ESP_LOGI(USB_API_TAG, "MSC storage deinitialization");
  tinyusb_msc_storage_deinit();
}

esp_err_t usb_msc_mount(usb_msc_config_t* config) {
  esp_err_t ret = ESP_OK;
  ret |= tinyusb_msc_storage_mount(config->mount_path);
  if (ret != ESP_OK) {
    ESP_LOGE(USB_API_TAG, "MSC Storage mount failed");
    return ret;
  }
  ESP_LOGI(USB_API_TAG, "MSC Storage mounted");
  return ret;
}

esp_err_t usb_msc_unmount() {
  esp_err_t ret = ESP_OK;
  ret |= tinyusb_msc_storage_unmount();
  if (ret != ESP_OK) {
    ESP_LOGE(USB_API_TAG, "MSC Storage unmount failed");
    return ret;
  }
  ESP_LOGI(USB_API_TAG, "MSC Storage unmounted");
  return ret;
}

bool usb_msc_check_storage_use_by_host() {
  return tinyusb_msc_storage_in_use_by_usb_host();
}
