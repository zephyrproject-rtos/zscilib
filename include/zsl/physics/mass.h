/*
 * Copyright (c) 2019-2020 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @addtogroup CENTER_OF_MASS Center of mass
 *
 * @brief Center of mass functions.
 *
 * @ingroup PHYSICS
 *  @{ */

/**
 * @file
 * @brief API header file for center of mass in zscilib.
 *
 * This file contains the zscilib center of mass APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_CMASS_H_
#define ZEPHYR_INCLUDE_ZSL_CMASS_H_

#include <zsl/zsl.h>
#include <zsl/vectors.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Calculates the three coordinate position of the center of mass of a
 *        set of objects given their masses and positions.
 *
 * @param m     Vector that contains the mass of each object in kilograms.
 * @param x     Vector that contains the x coordinate of each object in meters.
 * @param y     Vector that contains the y coordinate of each object in meters.
 * @param z     Vector that contains the z coordinate of each object in meters.
 * @param mx    Pointer to the x position of the center of mass in meters.
 * @param my    Pointer to the y position of the center of mass in meters.
 * @param mz    Pointer to the z position of the center of mass in meters.
 *
 * @return 0 if everything executed properly, -EINVAL if any of the masses has
 *         a negative value or the total mass is zero.
 */
int zsl_phy_mass_center(struct zsl_vec *m, struct zsl_vec *x,
			struct zsl_vec *y, struct zsl_vec *z, zsl_real_t *mx,
			zsl_real_t *my, zsl_real_t *mz);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_CMASS_H_ */

/** @} */ /* End of center of mass group */
