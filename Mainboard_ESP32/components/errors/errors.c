// cOPYRIGHT 2022 PWr in Space, Kuba
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "errors.h"
#include <memory.h>
#include "esp_log.h"

#define TAG "ERROR"

static struct {
    error_data_t errors_data[MAX_NUMBER_OF_ERRORS];
    size_t number_of_errors;
    SemaphoreHandle_t data_mutex;
} gb = {
    .data_mutex = NULL,
};

bool errors_init(error_type_t *errors_types, size_t number_of_errors) {
    if (number_of_errors > MAX_NUMBER_OF_ERRORS) {
        ESP_LOGE(TAG, "Maximum number of errors %d", number_of_errors);
        return false;
    }

    gb.data_mutex = xSemaphoreCreateMutex();
    if (gb.data_mutex == NULL) {
        return false;
    }

    for (int i = 0; i < number_of_errors; ++i) {
        if (errors_types[i] >= MAX_NUMBER_OF_ERRORS) {
            ESP_LOGE(TAG,
                    "Errors index may be repeated -> hashing function x %% MAX_NUMBER_OF_ERRORS");
            return false;
        }
    }

    gb.number_of_errors = number_of_errors;

    return true;
}

inline static uint8_t hash_function(uint8_t key) {
    return key % MAX_NUMBER_OF_ERRORS;
}

bool errors_add(error_type_t type, error_code_t code, uint32_t timeout) {
    if (gb.data_mutex == NULL) {
        return false;
    }

    if (xSemaphoreTake(gb.data_mutex, pdMS_TO_TICKS(timeout))) {
        return false;
    }
    gb.errors_data[hash_function(type)] |= code;

    xSemaphoreGive(gb.data_mutex);
    return true;
}