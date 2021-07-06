/*
 * Copyright (c) 2019-2020 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @addtogroup ROTATION Rotation
 *
 * @brief Rotation functions.
 *
 * @ingroup PHYSICS
 *  @{ */

/**
 * @file
 * @brief API header file for rotation in zscilib.
 *
 * This file contains the zscilib rotation APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_ROTATION_H_
#define ZEPHYR_INCLUDE_ZSL_ROTATION_H_

#include <zsl/zsl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Calculates the change in the angle based on initial angular velocity,
 *        time and acceleration.
 *
 * @param oi    Initial angular velocity in radians per second.
 * @param time  Time in seconds.
 * @param a     Angular acceleration in radians per second squared.
 * @param phi   Pointer to the placeholder for calculated change in the angle in
 *              radians. Will be set to NAN if time is a negative value.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_rot_angle(zsl_real_t oi, zsl_real_t time, zsl_real_t a,
		      zsl_real_t *phi);

/**
 * @brief Calculates the distance travelled in meters by an object under
 *        circular motion given the change in angle in radians (phi) and the
 *        radius of its trajectory (r).
 *
 * @param phi   Total change in angle by the object in radians.
 * @param r     Radius of the circular trajectory in meters.
 * @param dist  Pointer to the output distance in meters. Will be set to NAN
 *              if r is a negative value.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_rot_dist(zsl_real_t phi, zsl_real_t r, zsl_real_t *dist);

/**
 * @brief Calculates the number of rotations around a circle by an object under
 *        circular motion, based on the total change in angle it has travelled
 *        in radians (phi).
 *
 * @param phi   Total change in angle travelled by the object in radians.
 * @param turn  Pointer to the output number of rotations.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_rot_turn(zsl_real_t phi, zsl_real_t *turn);

/**
 * @brief Calculates the time in seconds that it takes an object to change its
 *        angular velocity from 'oi' to 'of' under a constant angular
 *        acceleration (a).
 *
 * @param oi    Initial angular velocity in radians per second.
 * @param of    Final angular velocity in radians per second.
 * @param a     Angular acceleration in radians per second squared.
 * @param time  Pointer to the placeholder for calculated time in seconds. Will
 *              be set to NAN if the time is negative or if a is zero.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_rot_time(zsl_real_t oi, zsl_real_t of, zsl_real_t a,
		     zsl_real_t *time);

/**
 * @brief Calculates the final angular velocity in radians per second of an
 *        object under a constant angular acceleration (a) based on its initial
 *        angular velocity (oi) and the time passed since it started
 *        moving (time).
 *
 * @param oi    Initial angular velocity in radians per second.
 * @param time  Time in seconds.
 * @param a     Angular acceleration in radians per second squared.
 * @param of    Pointer to the output final angular velocity in radians per
 *              second. Will be set to NAN if the time is negative.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_rot_omega(zsl_real_t oi, zsl_real_t time, zsl_real_t a,
		      zsl_real_t *of);

/**
 * @brief Calculates the final angular velocity in radians per second of an
 *        object under a constant angular acceleration (a) based on its initial
 *        angular velocity (oi) and the angle it has run (phy).
 *
 * @param oi    Initial angular velocity in radians per second.
 * @param phi   Angle run by the object in radians.
 * @param a     Angular acceleration in radians per second squared.
 * @param of    Pointer to the output final angular velocity in radians per
 *              second. Will be set to NAN if the angular velocity takes a
 *              complex value.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_rot_omega2(zsl_real_t oi, zsl_real_t phi, zsl_real_t a,
		       zsl_real_t *of);

/**
 * @brief Calculates the average angular velocity in radians per second of an
 *        object under circular motion based on the time and the angle it
 *        has run (phy).
 *
 * @param phi    Angle run by the object in radians.
 * @param time   Time in seconds.
 * @param omega  Pointer to the output average angular velocity in radians per
 *               second. Will be set to NAN if the time is negative or zero.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_rot_av_omega(zsl_real_t phi, zsl_real_t time, zsl_real_t *omega);

/**
 * @brief Calculates the module of the velocity tangent to the circular
 *        trajectory of an object under circular motion, based on its instant
 *        angular velocity (omega) and the radius of its trajectory (r).
 *
 * @param omega  Instant angular velocity in radians per second.
 * @param r      Radius of the circular trajectory in meters.
 * @param vel    Pointer to the output tangencial velocity in meters per
 *               second. Will be set to NAN if the radius is negative.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_rot_vel(zsl_real_t omega, zsl_real_t r, zsl_real_t *vel);

/**
 * @brief Calculates the angular acceleration of an object under circular
 *        motion in radians per second squared based on its initial (oi) and
 *        final (of) angular velocities and time.
 *
 * @param oi    Initial angular velocity in radians per second.
 * @param of    Final angular velocity in radians per second.
 * @param time  Time in seconds.
 * @param a     Pointer to the output angular acceleration in radians per
 *              second. Will be set to NAN if the time is negative or zero.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_rot_ang_accel(zsl_real_t oi, zsl_real_t of, zsl_real_t time,
		          zsl_real_t *a);

/**
 * @brief Calculates the module of the acceleration tangent to the circular
 *        trajectory of an object under circular motion, based on its
 *        angular acceleration (a) and the radius of its trajectory (r).
 *
 * @param a      Angular acceleration in radians per second squared.
 * @param r      Radius of the circular trajectory in meters.
 * @param accel  Pointer to the output tangencial acceleration in meters per
 *               second squared. Will be set to NAN if the radius is negative.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_rot_accel(zsl_real_t a, zsl_real_t r, zsl_real_t *accel);

/**
 * @brief Calculates the angular kinetic energy in joules of an object under
 *        circular motion based on its angular velocity (omega) and its moment
 *        of inertia (i).
 *
 * @param omega  Angular velocity in radians per second.
 * @param i      Moment of inertia of the object in kilograms and meters
 *               squared.
 * @param rke    Pointer to the output angular kinetic energy in joules.
 *               Will be set to NAN if the moment of inertia is negative.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_rot_ener(zsl_real_t omega, zsl_real_t i, zsl_real_t *rke);

/**
 * @brief Calculates the period in seconds of an object under circular motion
 *        given its angular velocity (omega).
 *
 * @param omega  Angular velocity in radians per second.
 * @param t      Pointer to the output period in seconds. Will be set to NAN if
 *               the angular velocity is zero or negative.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_rot_period(zsl_real_t omega, zsl_real_t *t);

/**
 * @brief Calculates the frequency in herzs of an object under circular motion
 *        given its angular velocity (omega).
 *
 * @param omega  Angular velocity in radians per second.
 * @param f      Pointer to the output frequency in herzs. Will be set to NAN
 *               if the angular velocity is negative.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_rot_frequency(zsl_real_t omega, zsl_real_t *f);

/**
 * @brief Calculates the module of the centripetal (AKA normal) acceleration
 *        experienced by an object moving under circular motion, given its
 *        tangencial velocity (vel) and the radius of its trajectory (r).
 *
 * @param vel  Tangencial velocity in meters per second.
 * @param r    Radius of the circular trajectory in meters.
 * @param ca   Pointer to the output centripetal acceleration in meters per
 *             second squared. Will be set to NAN if the radius is negative
 *             or zero.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_rot_cent_accel(zsl_real_t vel, zsl_real_t r, zsl_real_t *ca);

/**
 * @brief Calculates the module of the centripetal (AKA normal) acceleration
 *        experienced by an object moving under circular motion, given its
 *        period (t) and the radius of its trajectory (r).
 *
 * @param t    Period in second.
 * @param r    Radius of the circular trajectory in meters.
 * @param ca   Pointer to the output centripetal acceleration in meters per
 *             second squared. Will be set to NAN if the period is negative
 *             or zero or if the radius is negative.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_rot_cent_accel2(zsl_real_t t, zsl_real_t r, zsl_real_t *ca);

/**
 * @brief Calculates the module of the total acceleration in meters per second
 *        squared of an object moving under circular motion combining its
 *        tangencial (accel) and centripetal (ca) accelerations.
 *
 * @param accel   Tangencial acceleration in meters per second squared.
 * @param ca      Centripetal acceleration in meters per second squared.
 * @param at      Pointer to the output total acceleration in meters per
 *                second squared.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_rot_total_accel(zsl_real_t accel, zsl_real_t ca, zsl_real_t *at);

/**
 * @brief Calculates the mechanical power in watts necessary to mantain a
 *        circular motion given its torque and angular velocity (omega).
 *
 * @param torque   Torque in kilograms and meters squared per second.
 * @param omega    Angular velocity in radians per second.
 * @param power    Pointer to the output total power in watts.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_rot_power(zsl_real_t torque, zsl_real_t omega, zsl_real_t *power);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_ROTATION_H_ */

/** @} */ /* End of rotation group */
