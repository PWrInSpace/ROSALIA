// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#include "mcu_memory_config.h"

#define MCU_MEMORY_TAG "MCU_MEMORY"

static mcu_spi_config_t* spi_config;

esp_err_t memory_init(mcu_memory_config_t* memory_config) {
  esp_err_t ret;
  ret = spi_bus_initialize(&memory_config->sd_card->spi_host,
                           &memory_config->spi_bus_config, SDSPI_DEFAULT_DMA);
  if (ret != ESP_OK) {
    ESP_LOGE(MCU_MEMORY_TAG, "Failed to initialize SPI bus.");
    return ret;
  }

  // Card detect pin initialization
  gpio_config_t io_conf = {
      .pin_bit_mask = 1ULL << sd_card.card_detect_pin,
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

  bool err = SD_init(&sd_card, &sd_card_config);
  if (err != true) {
    ESP_LOGE(MCU_MEMORY_TAG, "Failed to initialize SD card.");
    return ESP_FAIL;
  }
  return ret;
}

esp_err_t memory_usb_msc_activate(mcu_memory_config_t* memory_config,
                                  usb_msc_init_storage_type_t storage_type) {
  esp_err_t ret;
  ret = usb_msc_init(&memory_config->usb_msc_config, storage_type);
  if (ret != ESP_OK) {
    ESP_LOGE(MCU_MEMORY_TAG, "Failed to initialize USB MSC.");
    return ret;
  }
  return ret;
}

esp_err_t memory_usb_msc_deactivate(mcu_memory_config_t* memory_config) {
  esp_err_t ret;
  usb_msc_deinit(&memory_config->usb_msc_config);
  if (ret != ESP_OK) {
    ESP_LOGE(MCU_MEMORY_TAG, "Failed to deinitialize USB MSC.");
    return ret;
  }
  return ret;
}

void memory_config_mount_spi_config(mcu_memory_config_t* memory_config,
                                    mcu_spi_config_t* mcu_spi_config) {
  spi_config = mcu_spi_config;
}
