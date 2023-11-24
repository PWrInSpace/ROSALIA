// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#include "mcu_memory_config.h"

#define MCU_MEMORY_TAG "MCU_MEMORY"

static mcu_spi_config_t* spi_config;

esp_err_t memory_init(mcu_memory_config_t* memory_config) {
  esp_err_t ret;

  // Card detect pin initialization
  gpio_config_t io_conf = {
      .pin_bit_mask = 1ULL << memory_config->sd_card->card_detect_pin,
      .mode = GPIO_MODE_INPUT,
      .pull_up_en = GPIO_PULLUP_ENABLE,
      .pull_down_en = GPIO_PULLDOWN_DISABLE,
      .intr_type = GPIO_INTR_DISABLE,
  };
  ret = gpio_config(&io_conf);
  if (ret != ESP_OK) {
    ESP_LOGE(MCU_MEMORY_TAG, "Failed to initialize card detect pin.");
    return ret;
  }

  // bool err = SD_init(memory_config->sd_card, memory_config->sd_card_config);
  // if (err != true) {
  //   ESP_LOGE(MCU_MEMORY_TAG, "Failed to initialize SD card.");
  //   return ESP_FAIL;
  // }

  FlashResult flash_result = FLASH_init(MAX_FILES);
  if (flash_result != FLASH_OK) {
    ESP_LOGE(MCU_MEMORY_TAG, "Failed to initialize SPI flash.");
    return ESP_FAIL;
  }
  return ret;
}

esp_err_t memory_deinit(mcu_memory_config_t* memory_config) {
  esp_err_t ret;
  bool bret;
  // bret = SD_unmount(memory_config->sd_card);
  // if (bret != true) {
  //   ESP_LOGE(MCU_MEMORY_TAG, "Failed to deinitialize SD card.");
  //   return ESP_FAIL;
  // }

  FlashResult flash_result = FLASH_deinit();
  if (flash_result != FLASH_OK) {
    ESP_LOGE(MCU_MEMORY_TAG, "Failed to deinitialize SPI flash.");
    return ESP_FAIL;
  }

  ret = spi_deinit(spi_config);
  if (ret != ESP_OK) {
    ESP_LOGE(MCU_MEMORY_TAG, "Failed to deinitialize SPI.");
    return ret;
  }

  return ret;
}

esp_err_t memory_usb_msc_activate(mcu_memory_config_t* memory_config,
                                  usb_msc_init_storage_type_t storage_type) {
  esp_err_t ret;
  // bool bret = SD_unmount(memory_config->sd_card);
  // if (bret != true) {
  //   ESP_LOGE(MCU_MEMORY_TAG, "Failed to unmount SD card.");
  //   return ESP_FAIL;
  // }
  ret = usb_msc_init(memory_config->usb_msc_config, storage_type);
  if (ret != ESP_OK) {
    ESP_LOGE(MCU_MEMORY_TAG, "Failed to initialize USB MSC.");
    return ret;
  }
  ret = usb_msc_mount(memory_config->usb_msc_config);
  if (ret != ESP_OK) {
    ESP_LOGE(MCU_MEMORY_TAG, "Failed to unmount USB MSC.");
    return ret;
  }
  return ret;
}

void memory_usb_msc_deactivate(mcu_memory_config_t* memory_config) {
  usb_msc_deinit(memory_config->usb_msc_config);
}

void memory_config_mount_spi_config(mcu_memory_config_t* memory_config,
                                    mcu_spi_config_t* mcu_spi_config) {
  spi_config = mcu_spi_config;
}
