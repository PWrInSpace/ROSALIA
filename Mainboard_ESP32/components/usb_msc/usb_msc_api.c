// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#include "usb_msc_api.h"

#define USB_API_TAG "USB_MSC_API"

esp_err_t usb_msc_init(usb_msc_config_t* config,
                       usb_msc_init_storage_type_t storage_type) {
  esp_err_t ret = ESP_OK;
  switch (storage_type) {
    case USB_MSC_INIT_STORAGE_TYPE_SDMMC:
      ret |= tinyusb_msc_storage_init_sdmmc(&config->sd_config);
      if (ret != ESP_OK) {
        ESP_LOGE(USB_API_TAG, "MSC Storage sdmmc init failed");
        return ret;
      }
      break;
    case USB_MSC_INIT_STORAGE_TYPE_SPI_FLASH:
      ret |= usb_msc_storage_init_spiflash(config);
      if (ret != ESP_OK) {
        ESP_LOGE(USB_API_TAG,
                 "MSC Storage spi flash init failed (wear levelling)");
        return ret;
      }
      ret |= tinyusb_msc_storage_init_spiflash(&config->spiflash_config);
      if (ret != ESP_OK) {
        ESP_LOGE(USB_API_TAG, "MSC Storage spi flash init failed");
        return ret;
      }
      break;
    default:
      ESP_LOGE(USB_API_TAG, "MSC Storage init failed - wrong storage type");
      return ESP_ERR_INVALID_ARG;
  }
  config->current_storage_type = storage_type;

  ESP_LOGI(USB_API_TAG, "MSC initialization");
  ret |= tinyusb_driver_install(&config->tusb_cfg);
  return ret;
}

void usb_msc_deinit(usb_msc_config_t* config) {
  ESP_LOGI(USB_API_TAG, "MSC storage deinitialization");
  if (config->current_storage_type == USB_MSC_INIT_STORAGE_TYPE_SPI_FLASH) {
    usb_msc_storage_deinit_spiflash(config);
  }
  tinyusb_msc_storage_deinit();
  config->current_storage_type = USB_MSC_INIT_STORAGE_NONE;
  tinyusb_driver_uninstall();
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

esp_err_t usb_msc_storage_init_spiflash(usb_msc_config_t* config) {
  ESP_LOGI(USB_API_TAG, "Initializing wear levelling");

  const esp_partition_t* data_partition = esp_partition_find_first(
      ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_FAT, NULL);
  if (data_partition == NULL) {
    ESP_LOGE(USB_API_TAG,
             "Failed to find FATFS partition. Check the partition table.");
    return ESP_ERR_NOT_FOUND;
  }

  return wl_mount(data_partition, &config->spiflash_config.wl_handle);
}

esp_err_t usb_msc_storage_deinit_spiflash(usb_msc_config_t* config) {
  ESP_LOGI(USB_API_TAG, "Deinitializing wear levelling");
  return wl_unmount(config->spiflash_config.wl_handle);
}
