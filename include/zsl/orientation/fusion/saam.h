/*
 * Copyright (c) 2021 Marti Riba Pons
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @defgroup FUSION_ALG_SAAM SAAM
 *
 * @brief SAAM sensor fuion algorithm.
 *
 * @ingroup FUSION_ALGORITHMS
 *  @{
 */

/**
 * @file
 * @brief SAAM sensor fusion algorithm.
 *
 * This file implements the SAAM sensor fusion algorithm.
 */

#ifndef ZEPHYR_INCLUDE_ZSL_FUSION_SAAM_H_
#define ZEPHYR_INCLUDE_ZSL_FUSION_SAAM_H_

#include <zsl/zsl.h>
#include <zsl/vectors.h>
#include <zsl/matrices.h>
#include <zsl/orientation/quaternions.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Source: https://hal.inria.fr/hal-01922922/document */

/**
 * @brief Sets the sample frequency (in Hz) for the algorithm.
 *
 * @param freq  Sampling frequency in Hz.
 * @param cfg   Config struct for this filter.
 *
 * @return int  0 if everything executed correctly, otherwise an appropriate
 *              negative error code.
 */
int zsl_fus_saam_init(uint32_t freq, void *cfg);

/**
 * @brief Super-fast Attitude from Accelerometer and Magnetometer (SAAM) sensor
 *        fusion algorithm implementation.
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
int zsl_fus_saam_feed(struct zsl_vec *a, struct zsl_vec *m,
		      struct zsl_vec *g, zsl_real_t *incl, struct zsl_quat *q,
		      void *cfg);

/**
 * @brief Default error handler for the SAAM sensor fusion driver.
 *
 * @note Normally you will want to implement your own version of this function.
 *
 * @param error     The error code genereated.
 */
void zsl_fus_saam_error(int error);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_FUSION_SAAM_H_ */

/** @} */ /* End of algorithms group */
