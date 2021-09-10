/*
 * Copyright (c) 2021 Kevin Townsend
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @defgroup COMPASS Compass
 *
 * @brief Compass functions based on magenetometer data.
 *
 * This modules provides functions to convert magnetometer readings -- ideally
 * from a data source that has been calibrated to remove hard-iron and
 * soft-iron errors -- into magnetic bearing and/or true bearing.
 *
 * Magnetic bearing is determined in relation to magnetic north, whose
 * position will vary from day to day depending on the latitude, longitude and
 * date.
 *
 * True (or geographic) bearing is determined in relation to true north --
 * sometimes referred to as geographic north -- which is the fixed, physical
 * north pole.
 *
 * The relationship between the true north and the shifting magnetic
 * north is called the 'absolute bearing', and is usually calculated as
 * a dms (decimal, minute, second) 'declination', given a specific latitude,
 * longitude and date. The calculated declination can then be applied to the
 * magnetic bearing measured by the magnetometer, and adjusted to account for
 * the difference (positive or negative) between the two.
 *
 * Several models exist to represent the current position of magnetic north
 * relative to true north, each with an associated range of time since the
 * models need to be updated over timee (i.e. WMM 2019-2024, IGRF 1590-2024,
 * EMM 2000-2019, etc.). These models are generally too complex to implement
 * in small embedded systems, so the 'declination' should be calculated using
 * an appropriate exterrnal tool, and used in the embedded systeem as an
 * updateable input.
 *
 * Since declination is usually represented in dms (rather than decimal
 * degrees), a helper function is provided to convert from dms (ex.:
 * 2° 13' 0" East) to decimal degrees (2.216667 in this case).
 *
 * @ingroup ORIENTATION
 *  @{
 */

/**
 * @file
 * @brief API header file for compass calculations in zscilib.
 *
 * This file contains the zscilib compass APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_COMPASS_H_
#define ZEPHYR_INCLUDE_ZSL_COMPASS_H_

#include <zsl/zsl.h>
#include <zsl/vectors.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Converts dms (degrees, minutes, seconds) to decimal degrees.
 *
 * Magnetic declination is most often represented in dms, so you may need to
 * convert dms to decimal degrees for use with the declination property in
 * @ref zsl_comp_geo_north.
 *
 * Note that the input 'd' value should be positive when east or north are
 * used, and negative with west or south. 'm' and 's' should always
 * be positive.
 *
 * @param d     Input dms degrees for latitude (negative for south and west).
 * @param m     Input dms minutes (always positive).
 * @param s     Input dms seconds (always positive).
 * @param dd    Pointer to the output decimal degrees value.
 *
 * @return 0 if everything executed normally, or a negative error code if the
 *         dimension of the magnetometer vector is not three.
 */
int zsl_comp_dms_to_dd(zsl_real_t d, zsl_real_t m, zsl_real_t s,
		       zsl_real_t *dd);

/**
 * @brief Calculates the magnetic north of the earth (not the geographical
 *        north) based on magnetometer data.
 *
 * This function assumes that the z axis of the magnetometer is orientated
 * upwards, and that the input magnetometer data has already been corrected
 * for hard and soft iron errors.
 *
 * The output of this function is an angle, which can be understood as the
 * angle between the magnetic north and the x axis of the magnetometer.
 *
 * Source:
 * https://cdn-shop.adafruit.com/datasheets/AN203_Compass_Heading_Using_Magnetometers.pdf
 *
 * @param m     Input corrected magnetometer tridimensional vector, in micro
 *              Tesla.
 * @param d     Output angle between the magnetic north and the x axis, in
 *              degrees.
 *
 * @return 0 if everything executed normally, or a negative error code if the
 * 		   dimension of the magnetometer vector is not three.
 */
int zsl_comp_magn_north(struct zsl_vec *m, zsl_real_t *d);

/**
 * @brief Calculates geographic north (AKA true bearing) based on the supplied
 *        magnetometer data, and the magentic declination.
 *
 * This function first calculates the magnetic bearing, determined based on the
 * supplied magnetometer values 'm', and adjusts it for the provided magnetic
 * declination 'dec' (in degrees), which represents the difference between
 * geographic north of the earth and magentic north at a given latitude and
 * date. The final result represents the true bearing or rather the deviation
 * from true north in degrees.
 *
 * The exact magnetic declination changes over time and location, and can't be
 * easily modeled on small embedded systems, so it must be determined from an
 * external source.
 *
 * To find the value of the magnetic declination angle based on the date,
 * latitude, longitude and altitude, one can use the following
 * online calculator:
 * https://www.ngdc.noaa.gov/geomag/calculators/magcalc.shtml?#igrfwmm
 *
 * This function assumes that the z axis of the magnetometer is orientated
 * upwards, and that the input magnetometer data has already been corrected
 * for hard and soft iron errors.
 *
 * @param m      Input magnetometer tridimensional vector, in micro Tesla.
 * @param dec    Input magnetic declination, in degrees.
 * @param b    	 Output angle deviation between true or geographic north and
 *               the x axis, in degrees (AKA true bearing).
 *
 * @return 0 if everything executed normally, or a negative error code if the
 *         dimension of the magnetometer vector is not three.
 */
int zsl_comp_geo_north(struct zsl_vec *m, zsl_real_t dec, zsl_real_t *b);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_COMPASS_H_ */

/** @} */ /* End of compass group */
