/*
 * Copyright (c) 2021 Marti Riba Pons
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @addtogroup ALGORITHMS
 *
 * @ingroup FUSION
 *  @{
 */

/**
 * @file
 * @brief Calibration.
 *
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
 * @brief Rotates the data from an acceletometer/magnetometer/gyroscope using
 * 		  a given rotation matrix.
 *
 * @param v 		Pointer to the tridimensional vector to rotate.
 * @param m 		Pointer to the 3x3 rotation matrix.
 * @param v_rot		Pointer to the output rotated vector.
 *
 * @return 0 if everything executed normally, or a negative error code if the
 * 		   dimension of the vectors or the square maatrix 'm' is not three.
 */
int zsl_fus_cal_rot_mtx(struct zsl_vec *v, struct zsl_mtx *m, 
						struct zsl_vec *v_rot);

/**
 * @brief Rotates the data from an acceletometer/magnetometer/gyroscope using
 * 		  a given axis and an angle.
 *
 * @param v 		Pointer to the tridimensional vector to rotate.
 * @param a 		Pointer to the rotation axis.
 * @param b 		Pointer to the angle of rotation, in radians.
 * @param v_rot		Pointer to the output rotated vector.
 *
 * @return 0 if everything executed normally, or a negative error code if the
 * 		   dimension of the vectors or the axis 'a' is not three.
 */
int zsl_fus_cal_rot_axis_angle(struct zsl_vec *v, struct zsl_vec *a,
						zsl_real_t *b, struct zsl_vec *v_rot);

#ifndef CONFIG_ZSL_SINGLE_PRECISION
/**
 * @brief This function calculates the soft iron 'K' and the hard iron 'b'
 *        errors in the magnetometer data, using the Levenberg–Marquardt
 *        algorithm and spherical and ellipsoid fitting.
 * 
 * Source: https://journals.sagepub.com/doi/full/10.1177/0020294019890627
 *
 * @param m 		Pointer to the input data Nx3 matrix, whose rows are
 * 					different magnetometer data samples.
 * @param l 		Damping parameter used to coltroll the iterative process.
 * 					Usually set to 1.
 * @param mu 		Controll parameter to increase or decrease lambda each step.
 * 					Must be bigger than one. Usually set to 10.
 * @param K			Pointer to the output soft iron error 3x3 symmetric matrix.
 * @param b			Pointer to the output hard iron error tridimensional vector.
 *
 * @return 0 if everything executed normally, or a negative error code if the
 * 		   dimension of the vector and matrices are invalid, or if mu < 1.
 */
int zsl_fus_cal_magn(struct zsl_mtx *m, zsl_real_t *l, zsl_real_t *mu,
		             struct zsl_mtx *K, struct zsl_vec *b);
#endif

/**
 * @brief Corrects the magnetometer data erasing the hard iron error (b) and
 * 		  the soft iron error (K).
 *
 * @param h 		Pointer to the input tridimensional magnetometer data vector 
 * 					to correct, in micro Tesla.
 * @param K 		Pointer to the input soft iron error 3x3 symmetric matrix.
 * @param b			Pointer to the input hard iron error tridimensional vector.
 * @param h_out		Pointer to the output corrected magnetometer data vector, 
 * 					in micro Tesla.
 *
 * @return 0 if everything executed normally, or a negative error code if the
 * 		   dimension of the vector defining the axis is not three.
 */
int zsl_fus_cal_magn_corr(struct zsl_vec *h, struct zsl_mtx *K,
						  struct zsl_vec *b, struct zsl_vec *h_out);

/**
 * @brief This function calculates the optimal value of the variable beta in
 * 		  the Madgwick filter, given a set of magnetometer, accelerometer and
 * 		  gyroscope data.
 * 
 * Source: http://iieng.org/images/proceedings_pdf/7697E0314552.pdf
 *
 * @param a   			 Input accelerometer Nx3 matrix. NULL if none.
 * @param m    			 Input magnetometer Nx3 matrix. NULL if none.
 * @param g    			 Input gyroscope Nx3 matrix. NULL if none.
 * @param sampleFreq	 Sampling frequency in Hz. Must be positive.
 * @param dip			 Input magnetic dip angle, in radians. NULL for none.
 * @param beta			 Output calculated optimal value of beta.
 *
 * @return 0 if everything executed normally, or a negative error code if the
 * 		   dimension of the matrices is not Nx3, or if the frequency is neg.
 */
int zsl_fus_cal_madg(struct zsl_mtx *g, struct zsl_mtx *a,
			struct zsl_mtx *m, zsl_real_t sampleFreq, zsl_real_t *dip,
			zsl_real_t *beta);

/**
 * @brief This function calculates the optimal value of the variable kp in
 * 		  the Mahony filter, given a set of magnetometer, accelerometer and
 * 		  gyroscope data. This function assumes that the optimal value of ki
 * 		  is 0.02.
 * 
 * Source: http://iieng.org/images/proceedings_pdf/7697E0314552.pdf
 *
 * @param a   			 Input accelerometer Nx3 matrix. NULL if none.
 * @param m    			 Input magnetometer Nx3 matrix. NULL if none.
 * @param g    			 Input gyroscope Nx3 matrix. NULL if none.
 * @param sampleFreq	 Sampling frequency in Hz. Must be positive.
 * @param dip			 Input magnetic dip angle, in radians. NULL for none.
 * @param beta			 Output calculated optimal value of beta.
 *
 * @return 0 if everything executed normally, or a negative error code if the
 * 		   dimension of the matrices is not Nx3, or if the frequency is neg.
 */
int zsl_fus_cal_mahn(struct zsl_mtx *g, struct zsl_mtx *a,
		    struct zsl_mtx *m, zsl_real_t sampleFreq, zsl_real_t *dip,
			zsl_real_t *kp);

/**
 * @brief This function calculates the magnetic north of the earth (not the
 * 	      geographical north) based on magnetometer data.
 * 
 * This function assumes hat the z axis of the magnetometer is orientated
 * upwards, and that the input magnetometer data has already been corrected
 * from hard andsoft iron errors.
 * 
 * The output of this function is an angle, which can be understood as the
 * angle between the magnetic north and the x axis of the magnetometer.
 * 
 * Source:
 * https://cdn-shop.adafruit.com/datasheets/AN203_Compass_Heading_Using_Magnetometers.pdf
 *
 * @param m  	 Input magnetometer tridimensional vector, in micro Tesla.
 * @param d    	 Output angle between the magnetic north and the x axis, in
 * 				 degrees.
 *
 * @return 0 if everything executed normally, or a negative error code if the
 * 		   dimension of the magnetometer vector is not three.
 */
int zsl_fus_cal_magn_north(struct zsl_vec *m, zsl_real_t *d);

/**
 * @brief This function calculates the geographic north of the earth (not the
 * 	      magnetic north) based on magnetometer data and deviation angle.
 * 
 * This function assumes hat the z axis of the magnetometer is orientated
 * upwards, and that the input magnetometer data has already been corrected
 * from hard andsoft iron errors.
 * 
 * The output of this function is an angle, which can be understood as the
 * angle between the magnetic north and the x axis of the magnetometer.
 * 
 * This function calculates the magnetic north and then takes into account the
 * magnetic deviation to calculate the true north, the geographical north. To
 * find the value of the magnetic deviation angle based on the date, latitude
 * and altitude, one can use the following online calculator:
 * https://www.ngdc.noaa.gov/geomag/calculators/magcalc.shtml?
 * 
 *
 * @param m  	 Input magnetometer tridimensional vector, in micro Tesla.
 * @param dev  	 Input magnetic deviation angle, in degrees.
 * @param d    	 Output angle between the magnetic north and the x axis, in
 * 				 degrees.
 *
 * @return 0 if everything executed normally, or a negative error code if the
 * 		   dimension of the magnetometer vector is not three.
 */
int zsl_fus_cal_geo_north(struct zsl_vec *m, zsl_real_t dev, zsl_real_t *d);


#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_FUSION_CALIBRATION_H_ */

/** @} */ /* End of calibration group */
