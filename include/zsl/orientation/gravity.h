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
 *  @{
 */

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
 * @brief Calculates the geographical gravitational field based on latitude and
 *        altitude over the sea level.
 *
 * Information taken from the following webpage:
 * https://www.welmec.org/welmec/documents/guides/2/2021/WELMEC_Guide_2_v2021.pdf
 *
 * @param lat   Latitude of the point where the gravity is to be calculated,
 *              in degrees.
 * @param alt   Altitude of the point of interest, in meters.
 * @param g     Pointer to the output gravity, in meters per second squared.
 *
 * @return 0 if everything executed correctly, -EINVAL if the altitude is
 *         negative or if the latitude is not between -90 and 90 degrees.
 */
int zsl_grav_lat_alt(zsl_real_t lat, zsl_real_t alt, zsl_real_t *g);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_GRAVITY_H_ */

/** @} */ /* End of gravity group */
