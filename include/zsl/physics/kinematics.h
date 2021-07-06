/*
 * Copyright (c) 2019-2020 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @addtogroup KINEMATICS Kinematics
 *
 * @brief Kinematics functions.
 *
 * @ingroup PHYSICS
 *  @{ */

/**
 * @file
 * @brief API header file for kinematics in zscilib.
 *
 * This file contains the zscilib kinematics APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_KINEMATICS_H_
#define ZEPHYR_INCLUDE_ZSL_KINEMATICS_H_

#include <zsl/zsl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Calculates the change in distance based on initial velocity,
 *        time and acceleration.
 *
 * @param vi    Initial velocity in meters per second.
 * @param time  Time in seconds.
 * @param accel Acceleration in meters per second squared.
 * @param dist  Pointer to the placeholder for calculated distance in meters.
 *              Will be set to NAN if time is a negative value.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_kin_dist(zsl_real_t vi, zsl_real_t time, zsl_real_t accel,
		     zsl_real_t *dist);

/**
 * @brief Calculates the initial position of a moving body based on the final
 *        position (xf), the initial velocity (vi), acceleration (a) and
 *        time (t).
 *
 * @param vi    Initial velocity in meters per second.
 * @param t     Time in seconds.
 * @param a     Acceleration in meters per second squared.
 * @param xf    Final position in meters.
 * @param xi    Pointer to the placeholder for the initial position in meters.
 *              Will be set to NAN if time is a negative value.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_kin_init_pos(zsl_real_t vi, zsl_real_t t, zsl_real_t a,
			 zsl_real_t xf, zsl_real_t *xi);

/**
 * @brief Calculates the initial position of a moving body based on the final
 *        position (xf), the initial velocity (vi), the final velocity (vf) and
 *        acceleration (a).
 *
 * @param vi    Initial velocity in meters per second.
 * @param vf    Final velocity in meters per second.
 * @param a     Acceleration in meters per second squared.
 * @param xf    Final position in meters.
 * @param xi    Pointer to the placeholder for the initial position in meters.
 *              Will be set to NAN if the acceleration's value is zero.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_kin_init_pos2(zsl_real_t vi, zsl_real_t vf, zsl_real_t a,
			  zsl_real_t xf, zsl_real_t *xi);

/**
 * @brief Calculates the time in seconds that it takes an object to change its
 *        velocity from vi to vf under a constant acceleration (accel).
 *
 * @param vi    Initial velocity in meters per second.
 * @param vf    Final velocity in meters per second.
 * @param accel Acceleration in meters per second squared.
 * @param time  Pointer to the placeholder for calculated time in seconds. Will
 *              be set to NAN if the time is negative or if the acceleration
 *              is zero.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_kin_time(zsl_real_t vi, zsl_real_t vf, zsl_real_t accel,
		     zsl_real_t *time);

/**
 * @brief Calculates the velocity in meters per second of an object under a
 *        constant acceleration (accel) based on its initial velocity (vi) and
 *        the time passed since it started moving (time).
 *
 * @param vi    Initial velocity in meters per second.
 * @param time  Time in seconds.
 * @param accel Acceleration in meters per second squared.
 * @param vf    Pointer to the output velocity in meters per second. Will
 *              be set to NAN if the time is negative.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_kin_vel(zsl_real_t vi, zsl_real_t time, zsl_real_t accel,
		    zsl_real_t *vf);

/**
 * @brief Calculates the velocity in meters per second of an object under a
 *        constant acceleration (accel) based on its initial velocity (vi) and
 *        the distance traveled from the starting point (dist).
 *
 * @param vi    Initial velocity in meters per second.
 * @param dist  Distance in meters.
 * @param accel Acceleration in meters per second squared.
 * @param vf    Pointer to the output velocity in meters per second. Will
 *              be set to NAN if the velocity takes a complex value.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_kin_vel2(zsl_real_t vi, zsl_real_t dist, zsl_real_t accel,
		     zsl_real_t *vf);

/**
 * @brief Calculates the initial velocity in meters per second of an object
 *        under a constant acceleration (a) based on its final velocity (vf)
 *        and time (t).
 *
 * @param vf    Final velocity in meters per second.
 * @param a     Acceleration in meters per second squared.
 * @param t     Time in seconds.
 * @param vi    Pointer to the output initial velocity in meters per second.
 *              Will be set to NAN if the time is negative.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_kin_init_vel(zsl_real_t vf, zsl_real_t a, zsl_real_t t,
			 zsl_real_t *vi);

/**
 * @brief Calculates the initial velocity in meters per second of an object
 *        under a constant acceleration (a) based on the distance it has
 *        travelled (dist) and time (t).
 *
 * @param dist  Distance in meters.
 * @param a     Acceleration in meters per second squared.
 * @param t     Time in seconds.
 * @param vi    Pointer to the output initial velocity in meters per second.
 *              Will be set to NAN if the time is negative or zero.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_kin_init_vel2(zsl_real_t dist, zsl_real_t a, zsl_real_t t,
			  zsl_real_t *vi);

/**
 * @brief Calculates the initial velocity in meters per second of an object
 *        under a constant acceleration (a) based on the distance it has
 *        travelled (dist) and the final velocity (vf).
 *
 * @param vf    Final velocity in meters per second.
 * @param a     Acceleration in meters per second squared.
 * @param dist  Distance in meters.
 * @param vi    Pointer to the output initial velocity in meters per second.
 *              Will be set to NAN if the output velocity takes a complex value.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_kin_init_vel3(zsl_real_t vf, zsl_real_t a, zsl_real_t dist,
		          zsl_real_t *vi);

/**
 * @brief Calculates the average velocity in meters per second of a moving
 *        object based on the time and distance.
 *
 * @param dist  Distance in meters.
 * @param time  Time in seconds.
 * @param vel   Pointer to the output velocity in meters per second. Will
 *              be set to NAN if the time is negative or zero.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_kin_av_vel(zsl_real_t dist, zsl_real_t time, zsl_real_t *vel);

/**
 * @brief Calculates the acceleration of a moving body in meters per second
 *        squared based on its initial (vi) and final (vf) velocities and time.
 *
 * @param vi    Initial velocity in meters per second.
 * @param vf    Final velocity in meters per second.
 * @param time  Time in seconds.
 * @param accel Pointer to the output acceleration in meters per second
 *              squared. Will be set to NAN if the time is negative or zero.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_kin_accel(zsl_real_t vi, zsl_real_t vf, zsl_real_t time,
		      zsl_real_t *accel);

/**
 * @brief Calculates the kinetic energy in joules of a moving body based on its
 *        velocity (v) and mass (m).
 *
 * @param v     Velocity of the object in meters per second.
 * @param m     Mass of the object in kilograms.
 * @param ke    Pointer to the output kinetic energy in joules. Will be set to
 *              NAN if the mass is negative.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_kin_ener(zsl_real_t v, zsl_real_t m, zsl_real_t *ke);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_KINEMATICS_H_ */

/** @} */ /* End of kinematics group */
