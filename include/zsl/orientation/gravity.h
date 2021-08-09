/*
 * Copyright (c) 2021 Kevin Townsend
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @defgroup GRAVITY Gravity
 *
 * @brief Functions and structs for dealing with gravity.
 *
 * @ingroup ORIENTATION
 *  @{ */

/**
 * @file
 * @brief API header file for gravity in zscilib.
 *
 * This file contains the zscilib gravity orientation APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_GRAVITY_H_
#define ZEPHYR_INCLUDE_ZSL_GRAVITY_H_

#include <zsl/zsl.h>
#include <zsl/vectors.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Converts a three-axis accelerometer sample (in m/s^2) to roll and
 *        pitch.
 *
 * @param accel 	Acceleration triplet in m/s^2.
 * @param a 		Pointer the the output @ref zsl_attitude struct.
 *
 * @return 0 if everything executed correctly, otherwise a negative error code.
 */
int zsl_grav_lat_alt(zsl_real_t *lat, zsl_real_t *alt, zsl_real_t *g);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_GRAVITY_H_ */

/** @} */ /* End of gravity group */
