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
 * @brief Represents the orientation of a rigid body with respect to
 *        the world frame in terms of roll, pitch and yaw (heading),
 *        expressed in degrees.
 *
 * @note This is functionally identical to @ref zsl_euler, but attitude is
 *       usually repesented in degrees, not radians. This struct has been
 *       defined to differentiate between the two representations.
 */
struct zsl_attitude {
	union {
		struct {
			/** @brief Longitudinal axis in degrees. A positive rolling motion
			 *         lifts the left wing and lowers the right wing of an
			 *         airplane. */
			zsl_real_t roll;
			/** @brief Transverse axis in degrees. A positive pitching motion
			*         raises the nose of the aircraft and lowers the tail. */
			zsl_real_t pitch;
			/** @brief Vertical axis in degrees. A positive yawing motion moves
			 *         the nose of the aircraft to the right. AKA heading. */
			zsl_real_t yaw;
		};
		/** @brief Array-based access. */
		zsl_real_t idx[3];
	};
	union {
		struct {
			/** @brief If non-zero, indicates that #roll is invalid. */
			uint8_t roll_invalid : 1;
			/** @brief If non-zero, indicates that #pitch is invalid. */
			uint8_t pitch_invalid : 1;
			/** @brief If non-zero, indicates that #yaw is invalid. */
			uint8_t yaw_invalid : 1;
		} status;
		/** @brief Invalid field indicators. */
		uint8_t status_bits;
	};
};

/**
 * @brief Takes the values in the supplied @ref zsl_attitude, and assigns the
 *        same memory address to a @ref zsl_vec, allowing for vector functions
 *        to be used on the zsl_attitude values.
 *
 * @param a     Pointer to the zsl_attitude struct to use.
 * @param v     Pointer to the zsl_vec struct to use.
 *
 * @return 0 if everything executed correctly, otherwise a negative error code.
 */
int zsl_att_to_vec(struct zsl_attitude *a, struct zsl_vec *v);

/**
 * @brief Converts the input @ref zsl_attitude, expessed in degrees, to it's
 *        equivalent in radians in the output @ref zsl_euler.
 *
 * @param a     Pointer to the zsl_attitued struct to use.
 * @param e     Pointer to the zsl_euler struct to use.
 *
 * @return 0 if everything executed correctly, otherwise a negative error code.
 */
int zsl_att_to_euler(struct zsl_attitude *a, struct zsl_euler *e);

/**
 * @brief Converts the input @ref zsl_euler, expessed in radians, to it's
 *        equivalent in degrees in the output @ref zsl_attitude.
 *
 * @param e     Pointer to the zsl_euler struct to use.
 * @param a     Pointer to the zsl_attitued struct to use.
 *
 * @return 0 if everything executed correctly, otherwise a negative error code.
 */
int zsl_att_from_euler(struct zsl_euler *e, struct zsl_attitude *a);

/**
 * @brief Converts a three-axis accelerometer (in m/s^2) and a three-axis
 *        magnetometer sample (in micro-Tesla) to attitude.
 *
 * @param accel     Acceleration triplet in m/s^2.
 * @param mag       Magnetometer triplet in micro-Tesla.
 * @param a         Pointer the the output @ref zsl_attitude struct.
 *
 * @return 0 if everything executed correctly, otherwise a negative error code.
 */
int zsl_att_from_accelmag(struct zsl_vec *accel, struct zsl_vec *mag,
			  struct zsl_attitude *a);

/**
 * @brief Converts a three-axis accelerometer sample (in m/s^2) to roll and
 *        pitch.
 *
 * @param accel     Acceleration triplet in m/s^2.
 * @param a         Pointer the the output @ref zsl_attitude struct.
 *
 * @return 0 if everything executed correctly, otherwise a negative error code.
 */
int zsl_att_from_accel(struct zsl_vec *accel, struct zsl_attitude *a);

/**
 * @brief Calculates the angle between two accelerometers.
 *
 * @param a1    First acceleration triplet in m/s^2.
 * @param a2    First acceleration triplet in m/s^2.
 * @param b     Angle between the accelerometers, in radians.
 *
 * @return 0 if everything executed correctly, otherwise a negative error code
 *         if the accelerometer data is not tridimensional.
 */
int zsl_att_accel_angle(struct zsl_vec *a1, struct zsl_vec *a2, zsl_real_t *b);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_AHRS_H_ */

/** @} */ /* End of ahrs group */
