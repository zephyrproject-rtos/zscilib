/*
 * Copyright (c) 2021 Kevin Townsend
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @defgroup AHRS Attitude and Heading Reference System
 *
 * @brief Functions and structs for dealing with 'Attitude' and AHRS.
 *
 * Structs like @ref zsl_attitude are inteded to be used as 'last-mile'
 * represetnations, to convert Euler angles (in radians) to something more
 * human-friendly like degrees. Most calculations should make use of Euler
 * angles, and the richer API around them, with conversion to degrees as the
 * final step.
 *
 * @ingroup ORIENTATION
 *  @{ */

/**
 * @file
 * @brief API header file for attitude and AHRS in zscilib.
 *
 * This file contains the zscilib AHRS APIs.
 */

#ifndef ZEPHYR_INCLUDE_ZSL_AHRS_H_
#define ZEPHYR_INCLUDE_ZSL_AHRS_H_

#include <zsl/zsl.h>
#include <zsl/vectors.h>
#include <zsl/orientation/euler.h>

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

#endif /* ZEPHYR_INCLUDE_ZSL_AHRS_H_ */

/** @} */ /* End of ahrs group */
