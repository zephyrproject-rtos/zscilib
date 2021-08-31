/*
 * Copyright (c) 2021 Kevin Townsend
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_ZSL_ORIENT_TEST_DATA_H_
#define ZEPHYR_INCLUDE_ZSL_ORIENT_TEST_DATA_H_

#ifdef __cplusplus
extern "C" {
#endif

#define DATA_SAMPLES (201)

extern struct zsl_mtx zsl_fus_data_mag;
extern struct zsl_mtx zsl_fus_data_gyr;
extern struct zsl_mtx zsl_fus_data_acc;
extern struct zsl_mtx zsl_fus_data_magn_cal;

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_ORIENT_TEST_DATA_H_ */
