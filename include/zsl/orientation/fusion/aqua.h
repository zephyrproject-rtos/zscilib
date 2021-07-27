/*
 * Copyright (c) 2021 Marti Riba Pons
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @addtogroup ALGORITHMS
 *
 * @ingroup FUSION
 *  @{
 */

/**
 * @file
 * @brief AQUA sensor fusion algorithm.
 *
 * This file implements the AQUA sensor fusion algorithm.
 */

#ifndef ZEPHYR_INCLUDE_ZSL_FUSION_AQUA_H_
#define ZEPHYR_INCLUDE_ZSL_FUSION_AQUA_H_

#include <zsl/zsl.h>
#include <zsl/vectors.h>
#include <zsl/matrices.h>
#include <zsl/orientation/quaternions.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Config settings for the AQUA sensor fusion algorithm.
 */
struct zsl_fus_aqua_cfg {
	/**
	 * @brief
	 *
	 */
	zsl_real_t alpha;

	/**
	 * @brief
	 *
	 */
	zsl_real_t beta;

	/**
	 * @brief
	 *
	 */
	zsl_real_t e_a;

	/**
	 * @brief
	 *
	 */
	zsl_real_t e_m;
};

/**
 * @brief Sets the sample frequency (in Hz) for the algorithm.
 *
 * @param freq  Sampling frequency in Hz.
 * 
 * @return int  0 if everything executed correctly, otherwise an appropriate
 *              negative error code.
 */
int zsl_fus_aqua_init(uint32_t freq, void* cfg);

/**
 * @brief Algebraic Quaterion Algorithm (AQUA) sensor fusion algorithm
 *        implementation.
 *
 * @param a     Input accelerometer vector (3 samples required). NULL if none.
 * @param m     Input magnetometer vector (3 samples required). NULL if none.
 * @param g     Input gyroscope vector (3 samples required). NULL if none.
 * @param q     Pointer to the output @ref zsl_quat.
 *
 * @return int  0 if everything executed correctly, otherwise an appropriate
 *              negative error code.
 */
int zsl_fus_aqua_feed(struct zsl_vec *a, struct zsl_vec *m,
		      struct zsl_vec *g, struct zsl_quat *q, void *cfg);

/**
 * @brief Default error handler for the AQUA sensor fusion driver.
 *
 * @note Normally you will want to implement your own version of this function.
 *
 * @param error     The error code genereated.
 */
void zsl_fus_aqua_error(int error);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_FUSION_AQUA_H_ */

/** @} */ /* End of algorithms group */
