/*
 * Copyright (c) 2021 Marti Riba Pons
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @defgroup FUSION_ALG_COMP Complementary
 *
 * @brief Complementary sensor fuion algorithm.
 *
 * @ingroup FUSION_ALGORITHMS
 *  @{
 */

/**
 * @file
 * @brief Complementary sensor fusion algorithm.
 *
 * This file implements the complementary sensor fusion algorithm.
 */

#ifndef ZEPHYR_INCLUDE_ZSL_FUSION_COMPLEMENTARY_H_
#define ZEPHYR_INCLUDE_ZSL_FUSION_COMPLEMENTARY_H_

#include <zsl/zsl.h>
#include <zsl/vectors.h>
#include <zsl/matrices.h>
#include <zsl/orientation/quaternions.h>
#include <zsl/orientation/ahrs.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Source: https://ahrs.readthedocs.io/en/latest/filters/complementary.html */

/**
 * @brief Config settings for the complementary sensor fusion algorithm.
 */
struct zsl_fus_comp_cfg {
	/**
	 * @brief Value between 0 and 1 used to decide which part of the
	 *        complementary test is more important, the gyrpscope
	 *        predicted orientation or the accel/magpredicted orientation.
	 *        For more accurate gyroscope data, alpha should be closer to
	 *        zero. For more accurate accelerometer and magnetometer data,
	 *        alpha should be closer to 1.
	 */
	zsl_real_t alpha;
};

/**
 * @brief Sets the sample frequency (in Hz) for the algorithm.
 *
 * @param freq  Sampling frequency in Hz.
 * @param cfg   Config struct for this filter.
 *
 * @return int  0 if everything executed correctly, otherwise an appropriate
 *              negative error code.
 */
int zsl_fus_comp_init(uint32_t freq, void *cfg);

/**
 * @brief Complementary sensor fusion algorithm implementation.
 *
 * @param a     Input accelerometer vector (3 samples required). NULL if none.
 * @param m     Input magnetometer vector (3 samples required). NULL if none.
 * @param g     Input gyroscope vector (3 samples required). NULL if none.
 * @param incl  Input magnetic inclination, in degrees. NULL for none.
 * @param q     Pointer to the output @ref zsl_quat.
 * @param cfg   Pointer to the config struct for this algorithm.
 *
 * @return int  0 if everything executed correctly, otherwise an appropriate
 *              negative error code.
 */
int zsl_fus_comp_feed(struct zsl_vec *a, struct zsl_vec *m,
		      struct zsl_vec *g, zsl_real_t *incl, struct zsl_quat *q,
		      void *cfg);

/**
 * @brief Default error handler for the complementary sensor fusion driver.
 *
 * @note Normally you will want to implement your own version of this function.
 *
 * @param error     The error code genereated.
 */
void zsl_fus_comp_error(int error);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_FUSION_COMPLEMENTARY_H_ */

/** @} */ /* End of algorithms group */
