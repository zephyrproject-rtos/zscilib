/*
 * Copyright (c) 2021 Marti Riba Pons
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @defgroup FUSION_ALG_EK Extended Kalman
 *
 * @brief Extended Kalman sensor fuion algorithm.
 *
 * @ingroup FUSION_ALGORITHMS
 *  @{
 */

/**
 * @file
 * @brief Extended Kalman filter algorithm.
 *
 * This file implements the extended Kalman filter algorithm.
 */

#ifndef ZEPHYR_INCLUDE_ZSL_FUSION_KALMAN_H_
#define ZEPHYR_INCLUDE_ZSL_FUSION_KALMAN_H_

#include <zsl/zsl.h>
#include <zsl/vectors.h>
#include <zsl/matrices.h>
#include <zsl/orientation/quaternions.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Source: https://ahrs.readthedocs.io/en/latest/filters/ekf.html */

/**
 * @brief Config settings for the extended Kalman filter.
 */
struct zsl_fus_kalm_cfg {
	/**
	 * @brief Noise spectral density of the gyroscope, in rad/s.
	 */
	zsl_real_t var_g;

	/**
	 * @brief Noise spectral density of the accelerometer, in m/s.
	 */
	zsl_real_t var_a;

	/**
	 * @brief Noise spectral density of the magnetometer, in micro Tesla.
	 */
	zsl_real_t var_m;

	/**
	 * @brief The predicted error state covariance, which is updated
	 *        every iteration. Its first value (P0) is a 4x4 identity
	 *        matrix.
	 */
	struct zsl_mtx P;

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
int zsl_fus_kalm_init(uint32_t freq, void *cfg);

/**
 * @brief Extended Kalman filter algorithm implementation.
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
int zsl_fus_kalm_feed(struct zsl_vec *a, struct zsl_vec *m,
		      struct zsl_vec *g, zsl_real_t *incl, struct zsl_quat *q,
		      void *cfg);

/**
 * @brief Default error handler for the extended Kalman filter driver.
 *
 * @note Normally you will want to implement your own version of this function.
 *
 * @param error     The error code genereated.
 */
void zsl_fus_kalm_error(int error);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_FUSION_KALMAN_H_ */

/** @} */ /* End of algorithms group */
