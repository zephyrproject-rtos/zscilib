/*
 * Copyright (c) 2019-2020 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @addtogroup DYNAMICS Dynamics
 *
 * @brief Dynamics functions.
 *
 * @ingroup PHYSICS
 *  @{ */

/**
 * @file
 * @brief API header file for dynamics in zscilib.
 *
 * This file contains the zscilib dynamics APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_DYNAMICS_H_
#define ZEPHYR_INCLUDE_ZSL_DYNAMICS_H_

#include <zsl/zsl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Calculates the force in newtons that experiences a body based on its
 *        mass (m) and acceleration (accel).
 *
 * @param m      Mass of the body in kilograms.
 * @param accel  Acceleration in meters per second squared.
 * @param f      Pointer to the output force in newtons. Will be set to NAN if
 *               the mass of the body is negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_dyn_newton(zsl_real_t m, zsl_real_t accel, zsl_real_t *f);

/**
 * @brief Calculates the friction force experienced by a body moving in a
 *        surface with a friction coefficient 'mu'.
 *
 * @param n     Normal force that the surface exerts to the body, in newtons.
 * @param mu    Friction coefficient, that ranges from zero to one.
 * @param f     Pointer to the output friction force in newtons. Will be set to
 *              NAN if 'mu' is less than zero or greater than 1.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_dyn_fric(zsl_real_t n, zsl_real_t mu, zsl_real_t *f);

/**
 * @brief Calculates the normal force in newtons exerted by plane inclined
 *        'theta' radians on a body of mass 'm'.
 *
 * @param m      Mass of the body in kilograms.
 * @param theta  Angle of inclination in radians.
 * @param n      Pointer to the output normal force in newtons. Will be set to
 *               NAN if the value of the body's mass is negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_dyn_normal(zsl_real_t m, zsl_real_t theta, zsl_real_t *n);

/**
 * @brief Calculates an approximation of the period of a simple pendulum with
 *        small oscilations, based on its length (l).
 *
 * @param l    Lenght of the pendulum in meters.
 * @param t    Pointer to the output period in seconds. Will be set to NAN if
 *             the lenght is negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_dyn_pendul_period(zsl_real_t l, zsl_real_t *t);

/**
 * @brief Calculates the maximum speed in meters per second of a simple pendulum
 *        based on its length (l) and initial angle (oi).
 *
 * @param l      Lenght of the pendulum in meters.
 * @param oi     Initial angle in radians.
 * @param vmax   Pointer to the output maximum speed in meters per second. Will
 *               be set to NAN if the lenght is negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_dyn_pendul_max_speed(zsl_real_t l, zsl_real_t oi, zsl_real_t *vmax);

/**
 * @brief Calculates the tension in newtons of a massless tensed rope with a
 *        moving object.
 *
 * @param m    Mass of the object in kilograms.
 * @param a    Acceleration of the object in the direction of the rope in
 *             meters per second squared.
 * @param t    Pointer to the output tension in newtons. Will be set to NAN if
 *             the mass of the object is negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_dyn_tension(zsl_real_t m, zsl_real_t a, zsl_real_t *t);

/**
 * @brief Calculates the force required to balance a lever, given the force
 *        applied to the other end of the lever and the distances between the
 *        end of the lever and its fulcrum.
 *
 * @param d1    Distance between the fulcrum and the first end of the lever.
 * @param f1    Force applied to the firts end of the lever in newtons.
 * @param d2    Distance between the fulcrum and the second end of the lever.
 * @param f2    Pointer to the output force in newtons. Will be set to NAN if
 *              any of the distances are negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_dyn_lever(zsl_real_t d1, zsl_real_t f1, zsl_real_t d2,
		      zsl_real_t *f2);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_DYNAMICS_H_ */

/** @} */ /* End of dynamics group */
