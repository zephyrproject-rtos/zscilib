/*
 * Copyright (c) 2019-2020 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @addtogroup MOMENTUM Momentum
 *
 * @brief Momentum functions.
 *
 * @ingroup PHYSICS
 *  @{ */

/**
 * @file
 * @brief API header file for momentum in zscilib.
 *
 * This file contains the zscilib momentum APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_MOMENTUM_H_
#define ZEPHYR_INCLUDE_ZSL_MOMENTUM_H_

#include <zsl/zsl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Calculates the linear momentum in kilograms and meters per second of
 *        an object based on its mass (m) and velocity (v).
 *
 * @param m    Mass of the object in kilograms.
 * @param v    Velocity in meters per second.
 * @param p    Pointer to the output linear momentum in kilograms and meters per
 *             second. Will be set to NAN if the mass of the object is negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_mom_mom(zsl_real_t m, zsl_real_t v, zsl_real_t *p);

/**
 * @brief Calculates the impulse given by a constant force acting during a
 *        certain period of time.
 *
 * @param f      Force applied in newtons.
 * @param time   Time in seconds.
 * @param imp    Pointer to the output impulse in newtons and seconds. Will be
 *               set to NAN if the time is negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_mom_imp(zsl_real_t f, zsl_real_t time, zsl_real_t *imp);

/**
 * @brief Calculates the gain or loss in momentum of an object based on its
 *        mass (m) and its initial (vi) and final (vf) velocities.
 *
 * @param m     Mass of the object in kilograms.
 * @param vi    Inital velocity in meters per second.
 * @param vf    Final velocity in meters per second.
 * @param p     Pointer to the output difference in momentum in kilograms and
 *              meters per second. Will be set to NAN if the mass is negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_mom_change(zsl_real_t m, zsl_real_t vi, zsl_real_t vf,
		       zsl_real_t *p);

/**
 * @brief Calculates the final velocity of an object after an elastic collision
 *        with another object given the final velocity of the other object and
 *        the masses and initial velocities of both objects.
 *
 * @param m1     Mass of the first object in kilograms.
 * @param vi1    Inital velocity of the first object in meters per second.
 * @param vf1    Final velocity of the first object in meters per second.
 * @param m2     Mass of the second object in kilograms.
 * @param vi2    Inital velocity of the second object in meters per second.
 * @param vf2    Pointer to the final velocity of the second object in meters
 *               per second. Will be set to NAN if any of the masses is
 *               negative or if the mass of the second object is zero.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_mom_elas(zsl_real_t m1, zsl_real_t vi1, zsl_real_t vf1,
		     zsl_real_t m2, zsl_real_t vi2, zsl_real_t *vf2);

/**
 * @brief Calculates the final velocity of the merged object after a perfect
 *        inelastic collision between two objects occur, given the masses and
 *        initial velocities of both the initial objects.
 *
 * @param m1     Mass of the first initial object in kilograms.
 * @param vi1    Inital velocity of the first object in meters per second.
 * @param m2     Mass of the second initial object in kilograms.
 * @param vi2    Inital velocity of the second object in meters per second.
 * @param vf     Pointer to the final velocity of the merged object in meters
 *               per second. Will be set to NAN if any of the masses is
 *               negative or if the sum of both masses is zero.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_mom_inelas(zsl_real_t m1, zsl_real_t vi1, zsl_real_t m2,
		       zsl_real_t vi2, zsl_real_t *vf);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_MOMENTUM_H_ */

/** @} */ /* End of momentum group */
