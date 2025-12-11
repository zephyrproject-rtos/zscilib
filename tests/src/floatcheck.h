/*
 * Copyright (c) 2019-2020 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Helper functions to test floating point values.
 *
 * This file contains helpers to test floating point values.
 */

#ifndef ZEPHYR_INCLUDE_ZSL_FLOATCHECK_H_
#define ZEPHYR_INCLUDE_ZSL_FLOATCHECK_H_

#include <zephyr/kernel.h>
#include <stdint.h>
#include <zsl/zsl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Checks if a zsl_real_t is equal to the specified value, with a +/- margin of
 * 'epsilon'.
 *
 * @param a         The value to check
 * @param b         The value to compare against
 * @param epsilon   The +/- margin for equality
 *
 * @return  True if value 'a' is equal to 'b', otherwise false.
 */
bool val_is_equal(zsl_real_t a, zsl_real_t b, zsl_real_t epsilon);

/**
 * Checks if a zsl_real_t is equal to or greater than the specified value.
 *
 * @param a         The value to check
 * @param b         The value to compare against
 *
 * @return  True if value 'a' is greater than or equal to 'b', otherwise false.
 */
bool val_is_at_least(zsl_real_t a, zsl_real_t b);

/**
 * Checks if a zsl_real_t is less than the specified value.
 *
 * @param a         The value to check
 * @param b         The value to compare against
 *
 * @return  True if value 'a' is less than 'b', otherwise false.
 */
bool val_is_less_than(zsl_real_t a, zsl_real_t b);

/**
 * Checks if a zsl_real_t is greater than the specified value.
 *
 * @param a         The value to check
 * @param b         The value to compare against
 *
 * @return  True if value 'a' is greater than 'b', otherwise false.
 */
bool val_is_greater_than(zsl_real_t a, zsl_real_t b);

/**
 * Checks if a zsl_real_t is within the range of 'upper' and 'lower'.
 *
 * @param a         The value to check
 * @param upper     The upper value to compare against (inclusive)
 * @param lower     The lower value to compare against (inclusive)
 *
 * @return  True if value 'a' is <= 'upper' and >='lower', otherwise false.
 */
bool val_is_within(zsl_real_t a, zsl_real_t upper, zsl_real_t lower);

#ifdef __cplusplus
}
#endif

#endif /* _MB_FLOATCHECK_H_ */
