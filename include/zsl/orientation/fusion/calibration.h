/*
 * Copyright (c) 2021 Marti Riba Pons
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @defgroup FUSION_CALIBRATION Calibration
 *
 * @brief Sensor fusion calibration
 *
 * @ingroup FUSION
 *  @{
 */

/**
 * @file
 * @brief Calibration helper functions.
 */

#ifndef ZEPHYR_INCLUDE_ZSL_FUSION_CALIBRATION_H_
#define ZEPHYR_INCLUDE_ZSL_FUSION_CALIBRATION_H_

#include <zsl/zsl.h>
#include <zsl/vectors.h>
#include <zsl/matrices.h>
#include <zsl/orientation/quaternions.h>
#include <zsl/orientation/ahrs.h>
#include <zsl/statistics.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Rotates accel/mag/gyro data using a given rotation matrix.
 *
 * @param v		Pointer to the tridimensional vector to rotate.
 * @param m		Pointer to the 3x3 rotation matrix.
 * @param v_rot	Pointer to the output rotated vector.
 *
 * @return 0 if everything executed normally, or a negative error code if the
 *         dimension of the vectors or the square maatrix 'm' is not three.
 */
int zsl_fus_cal_rot_mtx(struct zsl_vec *v, struct zsl_mtx *m,
			struct zsl_vec *v_rot);

/**
 * @brief Rotates accel/mag/gyro data using a given axis and angle.
 *
 * @param v     Pointer to the tridimensional vector to rotate.
 * @param a     Pointer to the rotation axis.
 * @param b     Pointer to the angle of rotation, in radians.
 * @param v_rot	Pointer to the output rotated vector.
 *
 * @return 0 if everything executed normally, or a negative error code if the
 *         dimension of the vectors or the axis 'a' is not three.
 */
int zsl_fus_cal_rot_axis_angle(struct zsl_vec *v, struct zsl_vec *a,
			       zsl_real_t *b, struct zsl_vec *v_rot);

#ifndef CONFIG_ZSL_SINGLE_PRECISION
/**
 * @brief This function calculates the soft iron ('K') and the hard iron
 *        ('b') errors in the magnetometer data, using the Levenberg–Marquardt
 *        algorithm and spherical and ellipsoid fitting.
 *
 * Source: https://journals.sagepub.com/doi/full/10.1177/0020294019890627
 *
 * @param m     Pointer to the input Nx3 matrix, whose rows consist of
 *              distinct magnetometer data samples.
 * @param l     Damping parameter used to control the iterative process.
 *              Typically set to 1.
 * @param mu    Control parameter used to increase or decrease lambda each
 *              step. Must be larger than one. Typically set to 10.
 * @param K     Pointer to the output soft iron error 3x3 symmetric matrix.
 * @param b     Pointer to the output hard iron error tridimensional vector.
 *
 * @return 0 if everything executed normally, or a negative error code if the
 *         dimension of the vector and matrices are invalid, or if mu < 1.
 */
int zsl_fus_cal_magn(struct zsl_mtx *m, zsl_real_t *l, zsl_real_t *mu,
		     struct zsl_mtx *K, struct zsl_vec *b);
#endif

#ifndef CONFIG_ZSL_SINGLE_PRECISION
/**
 * @brief This function calculates the soft iron ('K') and the hard iron
 *        ('b') errors in the magnetometer data, using the least squares method
 *        for ellipsoid fitting.
 *
 * This algorithm is faster than the previous one, which serves the same
 * function. In exchange, this function uses more memory.
 *
 * In this function, the module of the Earth's magnetic field at one's
 * location and date must be known. One can use the following web page to
 * obtain this parameter:
 * https://www.ngdc.noaa.gov/geomag/calculators/magcalc.shtml?#igrfwmm
 *
 * It will be displayed as the "Total Field", and should be converted from
 * nT to uT, so for a given value of 44387.0 nT, we could use 44.387 for 'me'.
 *
 * If you are unable to input or calculate an accurate value of 'me' yourself,
 * provide NULL to this argument and an approximation ot 50 uT will be used.
 *
 * Source of the algorithm:
 * https://www.researchgate.net/publication/331874483_Calibration_of_triaxial_magnetometer_with_ellipsoid_fitting_method
 *
 * @param m     Pointer to the input Nx3 matrix, whose rows consist of
 *              distinct magnetometer data samples.
 * @param me    Module of the magnetic field of the Earth at the current
 *              location (latitude, longitude and altitude) and date, in
 *              micro Tesla. If no value is known, provide NULL and an
 *              approximation will be used.
 * @param K     Pointer to the output soft iron error 3x3 symmetric matrix.
 * @param b     Pointer to the output hard iron error tridimensional vector.
 *
 * @return 0 if everything executed normally, or a negative error code if the
 *         dimension of the vector and matrices are invalid, or if me < 0.
 */
int zsl_fus_cal_magn_fast(struct zsl_mtx *m, zsl_real_t *me, struct zsl_mtx *K,
			  struct zsl_vec *b);
#endif

/**
 * @brief Corrects the supplied scalar number by adding another number ('b')
 *        and multiplying the sum by a scalar 'k'.
 *
 * @param d      Pointer to the input scalar to correct.
 * @param k      Pointer to the first input error scalar.
 * @param b      Pointer to the second input error scalar.
 * @param d_out  Pointer to the output corrected scalar.
 *
 * @return 0 if everything executed normally, or a proper negative error code.
 */
int zsl_fus_cal_corr_scalar(zsl_real_t *d, zsl_real_t *k, zsl_real_t *b,
			    zsl_real_t *d_out);

/**
 * @brief Corrects the supplied vector data by adding a vector the same
 *        size ('b') and then multiplying by a matrix 'K'.
 *
 * @param v      Pointer to the input data vector to correct.
 * @param K      Pointer to the input error matrix.
 * @param b      Pointer to the input error vector.
 * @param v_out  Pointer to the output corrected data vector.
 *
 * @return 0 if everything executed normally, or a negative error code if the
 *         dimension of the vectors and matrices don't match.
 */
int zsl_fus_cal_corr_vec(struct zsl_vec *v, struct zsl_mtx *K,
			 struct zsl_vec *b, struct zsl_vec *v_out);

/**
 * @brief Calculates the optimal value of the variable beta in
 *        the Madgwick filter, given a set of magnetometer, accelerometer and
 *        gyroscope readings.
 *
 * Source: http://iieng.org/images/proceedings_pdf/7697E0314552.pdf
 *
 * @param a             Input accelerometer Nx3 matrix. NULL if none.
 * @param m             Input magnetometer Nx3 matrix. NULL if none.
 * @param g             Input gyroscope Nx3 matrix. NULL if none.
 * @param sampleFreq    Sampling frequency in Hz. Must be positive.
 * @param incl          Input magnetic declination, in radians. NULL for none.
 * @param beta          Output calculated optimal value of beta.
 *
 * @return 0 if everything executed normally, or a negative error code if the
 *         dimension of the matrices is not Nx3, or if the frequency is neg.
 */
int zsl_fus_cal_madg(struct zsl_mtx *g, struct zsl_mtx *a,
		     struct zsl_mtx *m, zsl_real_t sampleFreq, zsl_real_t *incl,
		     zsl_real_t *beta);

/**
 * @brief Calculates the optimal value of the variable kp in
 *        the Mahony filter, given a set of magnetometer, accelerometer and
 *        gyroscope readings. This function assumes that the optimal value of
 *        ki is 0.02.
 *
 * Source: http://iieng.org/images/proceedings_pdf/7697E0314552.pdf
 *
 * @param a             Input accelerometer Nx3 matrix. NULL if none.
 * @param m             Input magnetometer Nx3 matrix. NULL if none.
 * @param g             Input gyroscope Nx3 matrix. NULL if none.
 * @param sampleFreq    Sampling frequency in Hz. Must be positive.
 * @param incl          Input magnetic declination, in radians. NULL for none.
 * @param kp            Output calculated optimal value of beta.
 *
 * @return 0 if everything executed normally, or a negative error code if the
 *         dimension of the matrices is not Nx3, or if the frequency is neg.
 */
int zsl_fus_cal_mahn(struct zsl_mtx *g, struct zsl_mtx *a,
		     struct zsl_mtx *m, zsl_real_t sampleFreq, zsl_real_t *incl,
		     zsl_real_t *kp);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_FUSION_CALIBRATION_H_ */

/** @} */ /* End of calibration group */
