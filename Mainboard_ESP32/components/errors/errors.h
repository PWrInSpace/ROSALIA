// Copyright 2022 PWrInSpace, Kuba
#ifndef ERRORS_H
#define ERRORS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// I'm 99 % sure it should be defined somewhere else but idk how to fix it 
#define CONFIG_MAX_NUMBER_OF_ERRORS 1
#define MAX_NUMBER_OF_ERRORS CONFIG_MAX_NUMBER_OF_ERRORS

/**
 * @brief error types should be enum, due to a shitty hash function
 * 
*/
typedef uint8_t error_type_t;
typedef uint8_t error_code_t;
typedef uint32_t error_data_t;

/**
 * @brief Initialize errors lib
 * 
 * @return true :D
 * @return false :C
*/
bool errors_init(error_type_t *error_types, size_t number_of_errors);

/**
 * @brief Set error, uses |= operator;
 * 
 * @param type error type
 * @param code error code
 * @return true :D
 * @return false :C
*/
bool errors_add(error_type_t type, error_code_t code, uint32_t timeout);

#endif