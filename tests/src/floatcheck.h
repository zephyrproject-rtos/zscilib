/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/**
 * @file
 * @brief Helper functions to test floating point values.
 *
 * This file contains helpers to test floating point values.
 */

#ifndef ZEPHYR_INCLUDE_ZSL_FLOATCHECK_H_
#define ZEPHYR_INCLUDE_ZSL_FLOATCHECK_H_

#include <zephyr.h>
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
 * @param b         The +/- margin for equality
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
