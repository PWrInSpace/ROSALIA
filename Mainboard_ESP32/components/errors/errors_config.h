// Copyright 2022 Pwr in Space, Kuba
#ifndef ERRORS_CONFIG_H
#define ERRORS_CONFIG_H

#include "errors.h"

typedef enum {
    ERROR_TYPE_ESP_NOW = 0,
    ERROR_TYPE_MEMORY,
    ERROR_TYPE_MCB,
    ERROR_TYPE_SENSORS,
    ERROR_TYPE_RECOVERY,
    ERROR_TYPE_LAST_EXCEPTION,
} errors_types_t;

typedef enum {
    ERROR_MEMORY_OK = 0,
    ERROR_SD_WRITE = (1 << 0),
    ERROR_FLASH_WRITE = (1 << 1),
    ERROR_FLASH_FILE = (1 << 3),
    ERROR_FLASH_FORMAT = (1 << 4),
    ERROR_FLASH_FULL = (1 << 5),
    ERROR_MEMORY_UNKNOWN = (1 << 6),
} errors_memory_t;

/**
 * @brief Initialize configured errors
 * 
 * @return true :D
 * @return false :C
*/
bool initialize_errors(void);

#endif