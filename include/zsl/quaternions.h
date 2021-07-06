/*
 * Copyright (c) 2019-2021 Kevin Townsend
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * \defgroup QUATERNIONS Quaternions
 *
 * @brief Quaternion math.
 *
 * @note These functions deal largely with unit quaternions. Further work is
 *       required to support arbitrary queternion math.
 *
 * Quaternions can be thought of in one of three ways:
 *
 * 1. As a vector with four components
 * 2. As a scalar (r) with a three component vector in right-hand ijk
 *    space (i, j, k)
 * 3. As a hypercomplex number consisting of a scalar real number (r),
 *    and three imaginary numbers (i, j, k)
 *
 * @note The component parts of quaternions are sometimes referred to
 * with a "w, x, y, z" notation instead of the "r, i, j, k" notation used in
 * zscilib. The real number (r or w) may also be positioned after the
 * imaginary numbers (xyzw or ijkr). The values are the same, but it's
 * important to maintain consistent ordering. An optional 'alias' field is
 * included in the zsl_quat struct if you prefer to use wxyz notation
 * (q.alias.w == q.r, etc.).
 *
 * With an extra degree of freedom compared to three-value Euler angles,
 * quaternions provide a compact and efficient means of rotating or orienting
 * vectors (x, y, z) in 3D space, similar to how vectors translate points
 * (x, y) in 2D space.
 *
 * They allow us to avoid a common problem with (otherwise more intuitive!)
 * Euler angles called "gimbal lock".
 *
 * Gimbal lock occurs when two axes in the Eular angle coincide after a
 * rotation by a multiple of 90 degrees about the third axis, resulting in the
 * loss of a degree of freedom. This leaves uss with an unclear value for the
 * logitude of the north or south poles, i.e. "which way is up?!?".
 *
 * Quaternions also enable not just LINEAR interpolation (lerp) between two
 * points (a and b) in normal space, but spherical linear interpolation (slerp)
 * between two points in a sphere, travelling across the surface of the
 * sphere rather than in a straight line. This enables us to interpolate
 * between arbitrary orientations (x, y, z), not just arbitrary points (x, y).
 */

/**
 * @file
 * @brief API header file for quaternions in zscilib.
 *
 * This file contains the zscilib quaternion APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_QUATERNIONS_H_
#define ZEPHYR_INCLUDE_ZSL_QUATERNIONS_H_

#include <zsl/zsl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Represents a quaternion, a 4D vector capable of representing 3D
 *        rigid body orientations.
 *
 * @note Quaternions can represent vectors by setting the scalar part (r) to 0.
 */
struct zsl_quat {
	union {
		struct {
			/** @brief The real component. Equal to "alias.w". */
			zsl_real_t r;
			/** @brief The first imaginary component. Equal to "alias.x". */
			zsl_real_t i;
			/** @brief The second imaginary component. Equal to "alias.y". */
			zsl_real_t j;
			/** @brief The third imaginary component. Equal to "alias.z". */
			zsl_real_t k;
		};
		struct {
			/** @brief The real component. Equal to "r". */
			zsl_real_t w;
			/** @brief The first imaginary component. Equal to "i". */
			zsl_real_t x;
			/** @brief The second imaginary component. Equal to "j". */
			zsl_real_t y;
			/** @brief The third imaginary component. Equal to "k". */
			zsl_real_t z;
		} alias;
		/** @brief Allows access to values as an array in rijk order. */
		zsl_real_t idx[4];
	};
};

/**
 * @brief Represents various 'types' of quaternions that can be initialised
 *        with the zsl_quat_init function.
 */
enum zsl_quat_type {
	/**
	 * @brief An empty quaternion where all values are set to 0.
	 */
	ZSL_QUAT_TYPE_EMPTY     = 0,
	/**
	 * @brief Indicates no rotation, with the scalar value set to 1 and
	 *        imaginary values set to 0.
	 */
	ZSL_QUAT_TYPE_IDENTITY  = 1,
};

/**
 * @brief Convenience function to set the supplied quaternion into a known
 *        state.
 *
 * @param q 	The quaterion to initialise.
 * @param type 	The memory layout to use when initialising the quaternion.
 */
void zsl_quat_init(struct zsl_quat *q, enum zsl_quat_type type);

/**
 * @brief Calculates the magnitude (or length) of the quaternion, a scalar
 *        value equal to the square root of the sum of the squares of the four
 *        components.
 *
 * @note A quaternion with a magnitude of 1 is referred to as a unit
 *       quaternion, and represents a rotation in 3D space.
 *
 * @param q The source quaternion.
 *
 * @return zsl_real_t The magnitude of q.
 */
zsl_real_t zsl_quat_magn(struct zsl_quat *q);

/**
 * @brief Normalises the input to be a "unit" or "rotation" quaternion, AKA
 *        a versor, such that the sum of the squares of the four componeents
 *        equals 1.0. This allows a quaternion to represent an rotation;
 *
 * Quaternion normalisation is required to represent rotation of objects
 * in 3D space, and for transforming orientation coordinates in said space.
 * You must normalise before using a quaternion to generate a rotation or
 * transformation matrix, or to rotate or transform a vector.
 *
 * Normalisation is accomplished by dividing each of the components by the
 * square root of the sum of the squares of the four quaternion components
 * (AKA the Euclidean norm).
 *
 * @param q 	The source quaternion.
 * @param qn 	The normalised output quaternion.
 *
 * @return 0 if everything executed normally, or a negative error code.
 */
int zsl_quat_to_unit(struct zsl_quat *q, struct zsl_quat *qn);

/**
 * @brief Normalises to unit quaternion (destructive).
 *
 * @param q 	The quaternion to normalise.
 *
 * @return 0 if everything executed normally, or a negative error code.
 */
int zsl_quat_to_unit_d(struct zsl_quat *q);

/**
 * @brief Verifies that this is a "unit" quaternion, meaning that it has a
 *        magnitude of 1, where sqrt(r^2+i^2+j^2+k^2) = 1.0.
 * 
 * @param q 	The quaternion to verify.
 * 
 * @return true		If this is a unit quaternion.
 * @return false 	If this is not a unit quaternion.
 */
bool zsl_quat_is_unit(struct zsl_quat *q);

/**
 * @brief Multiples each element in the quaternion by a scalar value.
 *
 * @param q 	The quaternion to scale.
 * @param s		The scalar value to use.
 * @param qs 	The scaled output quaternion.
 *
 * @return 0 if everything executed normally, or a negative error code.
 */
int zsl_quat_scale(struct zsl_quat *q, zsl_real_t s, struct zsl_quat *qs);

/**
 * @brief Multiples each element in the quaternion by a scalar value
 *        (destructive).
 *
 * @param q 	The quaternion to scale.
 * @param s		The scalar value to use.
 *
 * @return 0 if everything executed normally, or a negative error code.
 */
int zsl_quat_scale_d(struct zsl_quat *q, zsl_real_t s);

/**
 * @brief Multiples the two supplied unit quaternions (a * b) together, and
 *        stores the results in qm.
 *
 * @note Quaternion multiplication is not commutative, meaning that
 *       A*B != B*A. The order in which the quaternions appear when
 *       multiplying is important. To apply rotation q1, then rotation
 *       q2, we need to execute q3 = q2 * q1, in that order.
 *
 * @param qa 	The first input unit quaternion.
 * @param qb 	The second input unit quaternion.
 * @param qm 	The output placeholder.
 *
 * @return 0 if everything executed normally, or a negative error code.
 */
int zsl_quat_mult(struct zsl_quat *qa, struct zsl_quat *qb,
		  struct zsl_quat *qm);

/**
 * @brief Calculates the exponential of a unit quaternion.
 *
 * @param q 	The input unit quaternion.
 * @param qe 	The output unit quaternion.
 *
 * @return 0 if everything executed normally, or a negative error code.
 */
int zsl_quat_exp(struct zsl_quat *q, struct zsl_quat *qe);

/**
 * @brief Calculates the natural logarithm of a unit quaternion.
 *
 * @param q 	The input unit quaternion.
 * @param qe 	The output unit quaternion.
 *
 * @return zsl_real_t The logarithm of q.
 */
int zsl_quat_log(struct zsl_quat *q, struct zsl_quat *ql);

/**
 * @brief Multiples the supplied unit quaternions to power 'exp', and stores
 *        the results in qout (where qout = qa^pow).
 *
 * @param q 	The input unit quaternion.
 * @param exp 	The exponent to use, where qa^exp.
 * @param qout 	The output placeholder.
 *
 * @return 0 if everything executed normally, or a negative error code.
 */
int zsl_quat_pow(struct zsl_quat *q, zsl_real_t pow,
		 struct zsl_quat *qout);

/**
 * @brief Calculates the complex conjugate of 'q'.
 *
 * @param q 	The input quaternion.
 * @param qc 	The conjugate output;
 *
 * @return 0 if everything executed normally, or a negative error code.
 */
int zsl_quat_conj(struct zsl_quat *q, struct zsl_quat *qc);

/**
 * @brief Calculates the multiplicative inverse of unit quaternion 'q'.
 *
 * @param q 	The input unit quaternion.
 * @param qi 	The inverted output;
 *
 * @return 0 if everything executed normally, or a negative error code.
 */
int zsl_quat_inv(struct zsl_quat *q, struct zsl_quat *qi);

/**
 * @brief Inverts unit quaternion 'q' (destructive).
 *
 * @param q 	The input unit quaternion.
 *
 * @return 0 if everything executed normally, or a negative error code.
 */
int zsl_quat_inv_d(struct zsl_quat *q);

/**
 * @brief Calculates the rotation (qd) from qa to qb using the multiplicative
 *        inverse, such that qb = qa * qd.
 *
 * @param qa	The initial unit quaternion value.
 * @param qb	The final unit quaternion value.
 * @param qd	The difference as a quaternion.
 *
 * @return 0 if everything executed normally, or a negative error code.
 */
int zsl_quat_diff(struct zsl_quat *qa, struct zsl_quat *qb,
		  struct zsl_quat *qd);

/**
 * @brief Spherical linear interpolation.
 *
 * Calculates an intermediate rotation between qa and qb, based on the
 * provided interpolation factor, t (0.0..1.0). Output assigned to qi.
 *
 * @param qa	The starting unit quaternion value.
 * @param qb	The target unit quaternion value.
 * @param t     The interpolation factor (0.0..1.0)
 * @param qi	The interpolated quaternion.
 *
 * @return 0 if everything executed normally, or a negative error code.
 */
int zsl_quat_slerp(struct zsl_quat *qa, struct zsl_quat *qb,
		   zsl_real_t t, struct zsl_quat *qi);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_QUATERNIONS_H_ */

/** @} */ /* End of quaternions group */
