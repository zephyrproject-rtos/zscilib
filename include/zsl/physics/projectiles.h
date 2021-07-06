/*
 * Copyright (c) 2019-2020 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @addtogroup PROJECTILES Projectiles
 *
 * @brief Projectile functions.
 *
 * @ingroup PHYSICS
 *  @{ */

/**
 * @file
 * @brief API header file for projectiles in zscilib.
 *
 * This file contains the zscilib projectiles APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_PROJECTILES_H_
#define ZEPHYR_INCLUDE_ZSL_PROJECTILES_H_

#include <zsl/zsl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Decomposes the initial velocity of an object under projectile motion
 *        into its horizontal and vertical components based on the angle
 *        (theta) it is launched at.
 *
 * @param vi     Initial velocity of the object in meters per second.
 * @param theta  Angle between the floor and the initial velocity in radians.
 * @param vih    Pointer to the output horizontal component of the initial
 *               velocity in meters per second.
 * @param viv    Pointer to the output vertical component of the initial
 *               velocity in meters per second.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_proj_init_vel(zsl_real_t vi, zsl_real_t theta, zsl_real_t *vih,
			  zsl_real_t *viv);

/**
 * @brief Calculates the total time of travel in seconds of an object under
 *        projectile motion from an initial height (yi) and a final height (yf)
 *        given the vertical velocity at the starting point. Measurements are
 *        based on reaching yf on descent.
 *
 * @param viv    Initial vertical velocity of the object at the starting point
 *               in meters per second.
 * @param yi     Initial height in meters.
 * @param yf     Final height in meters.
 * @param t      Pointer to the output total time of flight of the object in
 *               seconds. Will be set to NAN if the object never reaches
 *               the final height.
 *
 * @return 0 if everything executed properly, -EINVAL if the final height is
 *         never reached in the projectile's trajectory.
 */
int zsl_phy_proj_time(zsl_real_t viv, zsl_real_t yi, zsl_real_t yf,
		      zsl_real_t *t);

/**
 * @brief Calculates the total time of travel in seconds of an object under
 *        projectile motion from an initial height (yi) and a final height (yf)
 *        given the vertical velocity at the starting point. Measurements are
 *        based on the first instance that yf is reached, which can occur
 *        during the ascent or descent stages depending on values provided.
 *
 * @param viv    Initial vertical velocity of the object at the starting point
 *               in meters per second.
 * @param yi     Initial height in meters.
 * @param yf     Final height in meters.
 * @param t      Pointer to the output total time of flight of the object in
 *               seconds. If t is a negative value of if the projectile doesn't
 *               reach y2, it will be set to NAN.
 *
 * @return 0 if everything executed properly, -EINVAL if the time of flight
 *         is negative or complex.
 */
int zsl_phy_proj_time_first(zsl_real_t viv, zsl_real_t yi, zsl_real_t yf,
			    zsl_real_t *t);

/**
 * @brief Calculates the total time of travel in seconds of an object under
 *        projectile motion based on the vertical velocity at the starting and
 *        ending points.
 *
 * @param viv    Initial vertical velocity of the object at the starting point
 *               in meters per second.
 * @param vfv    Final vertical velocity of the object at the ending point in
 *               meters per second.
 * @param t      Pointer to the output total time of flight of the object in
 *               seconds. If t is a negative value, it will be set to NAN.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_proj_time2(zsl_real_t viv, zsl_real_t vfv, zsl_real_t *t);

/**
 * @brief Calculates the height in meters of an object under projectile motion
 *        based on the initial vertical velocity (viv), the initial height (yi)
 *        and the time (t).
 *
 * @param viv    Initial vertical velocity of the object when launched in meters
 *               per second.
 * @param t      Time that has transcurred from the moment the object was
 *               launched, in seconds.
 * @param yi     Initial height of the object in meters.
 * @param yf     Pointer to the output height of the object in meters. If the
 *               time is negative, it will be set to NAN.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_proj_ver_motion(zsl_real_t viv, zsl_real_t t, zsl_real_t yi,
			    zsl_real_t *yf);

/**
 * @brief Calculates the vertical velocity in meters per second of an object
 *        under projectile motion based on the initial vertical velocity (viv),
 *        and the time (t).
 *
 * @param viv    Initial vertical velocity of the object when launched in meters
 *               per second.
 * @param t      Time that has transcurred from the moment the object was
 *               launched, in seconds.
 * @param vfv    Pointer to the output vertical velocity in meters per second.
 *               If the time is negative, it will be set to NAN.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_proj_ver_vel(zsl_real_t viv, zsl_real_t t, zsl_real_t *vfv);

/**
 * @brief Calculates the horizontal position in meters of an object under
 *        projectile motion based on the horizontal velocity (vih), the time (t)
 *        and the initial horizontal position (xi).

 * @param vih    Horizontal velocity of the object in meters per second.
 * @param t      Time that has transcurred from the moment the object was
 *               launched, in seconds.
 * @param xi     Horizontal initial position of the object in meters.
 * @param xf     Pointer to the output horizontal position in meters. If the
 *               time is negative, it will be set to NAN.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_proj_hor_motion(zsl_real_t vih, zsl_real_t t, zsl_real_t xi,
			    zsl_real_t *xf);

/**
 * @brief Calculates the vertical position in meters of an object under
 *        projectile motion at any given horizontal position, knowing the
 *        initial vertical velocity (viv), the horizontal velocity (vih) and
 *        the coordinates of the launching point (xi, yi).
 *
 * @param vih    Horizontal velocity of the object in meters per second.
 * @param viv    Initial vertical velocity of the object in meters per second.
 * @param xi     Horizontal initial position of the object in meters.
 * @param yi     Vertical initial position of the object in meters.
 * @param xf     Horizontal final position of the object in meters.
 * @param yf     Pointer to the output height in meters. If the horizontal
 *               velocity is zero it will be set to NAN.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_proj_trajectory(zsl_real_t vih, zsl_real_t viv, zsl_real_t xi,
			    zsl_real_t yi, zsl_real_t xf, zsl_real_t *yf);

/**
 * @brief Calculates the module of the total velocity in meters per second of
 *        an object under projectile motion at any point, given the vertical
 *        (vfv) and horizontal (vfh) velocities at that point.
 *
 * @param vfh    Horizontal velocity of the object in meters per second.
 * @param vfv    Vertical velocity of the object in meters per second.
 * @param vf     Pointer to the output total velocity in meters per second.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_proj_vel(zsl_real_t vfh, zsl_real_t vfv, zsl_real_t *vf);

/**
 * @brief Calculates the angle in radians between the floor and the total
 *        velocity of an object under projectile motion based on the vertical
 *        (vfv) and horizontal (vfh) velocities.
 *
 * @param vfh    Horizontal velocity of the object in meters per second.
 * @param vfv    Vertical velocity of the object in meters per second.
 * @param theta  Pointer to the output angle in radians.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_proj_angle(zsl_real_t vfh, zsl_real_t vfv, zsl_real_t *theta);

/**
 * @brief Calculates the horizontal distance in meters between the launching
 *        point of an object and the point where it reaches the ground based on
 *        the initial vertical velocity (viv), the horizontal velocity (vih)
 *        and the coordinates of the launching point (xi, yi).
 *
 * @param vih    Horizontal velocity of the object in meters per second.
 * @param viv    Initial vertical velocity of the object in meters per second.
 * @param xi     Horizontal initial position of the object in meters.
 * @param yi     Vertical initial position of the object in meters.
 * @param dist   Pointer to the output distance in meters.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_proj_range(zsl_real_t vih, zsl_real_t viv, zsl_real_t xi,
		       zsl_real_t yi, zsl_real_t *dist);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_PROJECTILES_H_ */

/** @} */ /* End of projectiles group */
