// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#pragma once

#include "flash.h"
#include "flash_nvs.h"
#include "sdcard.h"
#include "usb_msc_api.h"

typedef struct {
  sd_card_config_t sd_card_config;
  sd_card_t sd_card;
  usb_msc_config_t usb_msc_config;
} mcu_memory_config_t;
