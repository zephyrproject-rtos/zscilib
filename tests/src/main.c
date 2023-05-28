/*
 * Copyright (c) 2023 Kevin Townsend
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>

ZTEST_SUITE(zsl_tests, NULL, NULL, NULL, NULL, NULL);

#ifndef CONFIG_ZSL_SINGLE_PRECISION
ZTEST_SUITE(zsl_tests_double, NULL, NULL, NULL, NULL, NULL);
#endif