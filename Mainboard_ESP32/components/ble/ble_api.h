// Copyright 2023 PWr in Space, Krzysztof Gliwiński
#pragma once

#include <stdint.h>
#include "ble_gap_conf.h"
#include "ble_gatt_conf.h"

typedef struct {
  uint8_t *data;
  uint8_t len;
} ble_packet_t;
