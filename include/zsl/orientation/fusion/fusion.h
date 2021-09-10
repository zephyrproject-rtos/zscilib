/*
 * Copyright (c) 2021 Kevin Townsend
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @defgroup FUSION Sensor Fusion
 *
 * @brief Sensor fusion
 *
 * @ingroup ORIENTATION
 *  @{
 */

/**
 * @defgroup FUSION_ALGORITHMS Algorithms
 *
 * @brief Implementations of specific fusion algorithms
 *
 * @ingroup FUSION
 */

/**
 * @file
 * @brief API header file for sensor fusion algorithms in zscilib.
 *
 * This file contains the zscilib sensor fusion API.
 */

#ifndef ZEPHYR_INCLUDE_ZSL_FUSION_H_
#define ZEPHYR_INCLUDE_ZSL_FUSION_H_

#include <zsl/zsl.h>
#include <zsl/vectors.h>
#include <zsl/matrices.h>
#include <zsl/orientation/quaternions.h>
#include <zsl/orientation/fusion/madgwick.h>
#include <zsl/orientation/fusion/mahony.h>
#include <zsl/orientation/fusion/saam.h>
#include <zsl/orientation/fusion/aqua.h>
#include <zsl/orientation/fusion/complementary.h>
#include <zsl/orientation/fusion/kalman.h>
#include <zsl/orientation/fusion/calibration.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @typedef zsl_fus_init_cb_t
 * @brief Init callback prototype for sensor fusion implementations.
 *
 * @param freq      Sample frequency in Hz (samples per second).
 * @param cfg       Pointer to the config struct for this algorithm.
 *
 * @return 0 on success, negative error code on failure
 */
typedef int (*zsl_fus_init_cb_t)(uint32_t freq, void *cfg);

/**
 * @typedef zsl_fus_feed_cb_t
 * @brief Update callback prototype for sensor fusion implementations.
 *
 * Some filters have the option to include the Earth's magnetic field
 * inclination (the angle between the magnetic north and the vertical axis) at
 * a certain date, latitude, longitude and altitude for better results. This
 * filters are Madgwick, Mahony and Kalman. For the other filters or for
 * inexistent inclination data, use NULL. To calculate the inclination (also
 * known as dip angle) one can use the following webpage:
 * https://www.ngdc.noaa.gov/geomag/calculators/magcalc.shtml?#igrfwmm
 *
 *
 * @param accel     Pointer to the accelerometer XYZ data. NULL for none.
 * @param mag       Pointer to the magnetometer XYZ data. NULL for none.
 * @param gyro      Pointer to the gyroscope XYZ data. NULL for none.
 * @param incl      Pointer to the Earth's magnetic field inclination angle,
 *                  in degrees. NULL for none.
 * @param q         Pointer to the quaternion output placeholder.
 * @param cfg       Pointer to the config struct for this algorithm.
 *
 * @return 0 on success, negative error code on failure
 */
typedef int (*zsl_fus_feed_cb_t)(struct zsl_vec *accel,
				 struct zsl_vec *mag, struct zsl_vec *gyro,
				 zsl_real_t *incl, struct zsl_quat *q, void *cfg);

/**
 * @typedef zsl_fus_error_cb_t
 * @brief Callback prototype when a fusion algorithm fails to properly feed.
 *
 * @param error     Negative error code produced during node execution.
 */
typedef void (*zsl_fus_error_cb_t)(int error);

/**
 * @brief Sensor fusion algorithm implementation.
 */
struct zsl_fus_drv {
	/**
	 * @brief Callback to fire when initialising the driver.
	 */
	zsl_fus_init_cb_t init_handler;

	/**
	 * @brief Callback to fire when feeding/updating the driver.
	 */
	zsl_fus_feed_cb_t feed_handler;

	/**
	 * @brief Callback to fire when the 'feed' command fails.
	 */
	zsl_fus_error_cb_t error_handler;

	/**
	 * @brief Config settings for the driver. The exact struct or
	 *        value(s) defined here are driver-specific and defined
	 *        by the implementing module.
	 */
	void *config;
};

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_FUSION_H_ */

/** @} */ /* End of fusion group */
