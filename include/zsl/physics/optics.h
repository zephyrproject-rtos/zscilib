/*
 * Copyright (c) 2019-2020 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @addtogroup OPTICS Optics
 *
 * @brief Optics functions.
 *
 * @ingroup PHYSICS
 *  @{ */

/**
 * @file
 * @brief API header file for optics in zscilib.
 *
 * This file contains the zscilib optics APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_OPTICS_H_
#define ZEPHYR_INCLUDE_ZSL_OPTICS_H_

#include <zsl/zsl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Calculates index of refraction of a medium based on the velocity of
 *        light in the medium.
 *
 * @param v    Velocity of light in the medium in meters per second.
 * @param n    Pointer to the output index of refraction. Will be set to NAN if
 *             the velocity is negative or greater than the speed of light.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_opt_refrac_index(zsl_real_t v, zsl_real_t *n);

/**
 * @brief Calculates the angle of refraction of a light beam changing the medium
 *        of propagation based on the incident angle and the index of refraction
 *        of both mediums.
 *
 * @param n1    Index of refraction of the first medium.
 * @param n2    Index of refraction of the second medium.
 * @param o1    Angle of incision in radians.
 * @param o2    Pointer to the output angle of refraction in radians. Will be
 *              set to NAN if any of the indexes is negative or if the
 *              light beam does not change of medium.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_opt_snell(zsl_real_t n1, zsl_real_t n2, zsl_real_t o1,
		      zsl_real_t *o2);

/**
 * @brief Calculates the distance of the focus of a curved mirror based on the
 *        position of the real (sr) and imaginary (si) images.
 *
 * @param sr    Distance to the real image in meters.
 * @param si    Distance to the imaginary image in meters.
 * @param f     Pointer to the output distance to the focus in meters. Will be
 *              set to NAN if the mirror is plane AKA sr + si = 0.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_opt_focus(zsl_real_t sr, zsl_real_t si, zsl_real_t *f);

/**
 * @brief Calculates the critical angle of incision from which the refraction
 *        effect does not occur.
 *
 * @param n1    Index of refraction of the first medium.
 * @param n2    Index of refraction of the second medium.
 * @param oc    Pointer to the output critical angle in radians. Will be set to
 *              NAN if any of the indexes is negative or if n2 > n1.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_opt_critic_angle(zsl_real_t n1, zsl_real_t n2, zsl_real_t *oc);

/**
 * @brief Calculates the power of a lens based on its focal length.
 *
 * @param f    Focal length in meters.
 * @param p    Pointer to the output power in 1 / meter. Will be set to NAN if
 *             f = 0.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_opt_power(zsl_real_t f, zsl_real_t *p);

/**
 * @brief Calculates the magnification of a lens based on the real length of an
 *        object and its apparent length through the lens.
 *
 * @param y1    Real length of the object in meters.
 * @param y2    Apparent length of the object in meters.
 * @param m     Pointer to the output magnification value. Will be set to NAN if
 *              any of the input values is negative of if y1 = 0.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_opt_magn(zsl_real_t y1, zsl_real_t y2, zsl_real_t *m);

/**
 * @brief Calculates the distance between two objects when a light beam of
 *        wavelength comparable to the distance passes through them and
 *        difracts producing spectral lines.
 *
 * @param n        Number of the spectral line.
 * @param lambda   Wavelength of the light beam in nanometers.
 * @param o        Angle between the center of the distance and the n-th
 *                 spectral line in radians.
 * @param d        Pointer to the output distance in nanometers. Will be set to
 *                 NAN if either 'n', 'lambda' or sin(o) are negative or if
 *                 sin(o) = 0.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_opt_dif(uint8_t n, zsl_real_t lambda, zsl_real_t o, zsl_real_t *d);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_OPTICS_H_ */

/** @} */ /* End of optics group */
