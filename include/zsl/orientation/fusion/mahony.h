/*
 * Copyright (c) 2021 Marti Riba Pons
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @defgroup FUSION_ALG_MAHN Mahoney
 *
 * @brief Mahoney sensor fuion algorithm.
 *
 * @ingroup FUSION_ALGORITHMS
 *  @{
 */

/**
 * @file
 * @brief Mahony sensor fusion algorithm.
 *
 * This file implements the Mahony sensor fusion algorithm.
 */

#ifndef ZEPHYR_INCLUDE_ZSL_FUSION_MAHONY_H_
#define ZEPHYR_INCLUDE_ZSL_FUSION_MAHONY_H_

#include <zsl/zsl.h>
#include <zsl/vectors.h>
#include <zsl/matrices.h>
#include <zsl/orientation/quaternions.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Source: https://ahrs.readthedocs.io/en/latest/filters/mahony.html */

/**
 * @brief Config settings for the Mahony sensor fusion algorithm.
 */
struct zsl_fus_mahn_cfg {
	/**
	 * @brief Proportional filter gain constant. Must be greater than 0.
	 */
	zsl_real_t kp;

	/**
	 * @brief Integral filter gain constant. Must be greater than 0.
	 */
	zsl_real_t ki;

	/**
	 * @brief Integral limit for the integrator to avoid windup. Must be greater than 0
	 */
	zsl_real_t integral_limit;
	
	/**
	 * @brief Integral feedback vector, which is updated every iteration.
	 *        Its initial value must be (0, 0, 0).
	 */
	struct zsl_vec intfb;
};

/**
 * @brief Sets the sample frequency (in Hz) for the algorithm.
 *
 * @param freq  Sampling frequency in Hz.
 * @param cfg   Pointer to the config struct for this algorithm.
 *
 * @return int  0 if everything executed correctly, otherwise an appropriate
 *              negative error code.
 */
int zsl_fus_mahn_init(uint32_t freq, void *cfg);

/**
 * @brief Mahony sensor fusion algorithm implementation.
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
int zsl_fus_mahn_feed(struct zsl_vec *a, struct zsl_vec *m,
		      struct zsl_vec *g, zsl_real_t *incl, struct zsl_quat *q,
		      void *cfg);

/**
 * @brief Default error handler for the Mahony sensor fusion driver.
 *
 * @note Normally you will want to implement your own version of this function.
 *
 * @param error     The error code genereated.
 */
void zsl_fus_mahn_error(int error);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_FUSION_MAHONY_H_ */

/** @} */ /* End of algorithms group */
