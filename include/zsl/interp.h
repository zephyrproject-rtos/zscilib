/*
 * Copyright (c) 2019-2020 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @defgroup INTERPOLATE Interpolation
 *
 * @brief Various functions for linear, cubic spline and nearest-neighbour
 * interpolation of X,Y data.
 */

/**
 * @file
 * @brief API header file for interpolation functions in zscilib.
 *
 * This file contains the zscilib interpolation APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_INTERP_H_
#define ZEPHYR_INCLUDE_ZSL_INTERP_H_

#include <stdint.h>
#include <zsl/zsl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup INTERP_STRUCTS Structs and Enums
 *
 * Common interpolation-related structs and enums.
 *
 * @ingroup INTERPOLATE
 *  @{ */

/** @brief XY struct for nearest neighbour and linear interpolation. */
struct zsl_interp_xy {
	zsl_real_t x;
	zsl_real_t y;
};

/** @brief XY struct for cubic spline interpolation. */
struct zsl_interp_xyc {
	zsl_real_t x;
	zsl_real_t y;
	zsl_real_t y2; /**< @brief Second derivative from the spline. */
};

/** @} */ /* End of INTERP_STRUCTS group */

/**
 * @addtogroup INTERP_FUNCS Functions
 *
 * Helper functions to interpolate between known values.
 *
 * @ingroup INTERPOLATE
 *  @{ */

/**
 * @brief Calculates a number between two numbers using linear interpolation,
 * where 't' is the interpolation factor between 0.0 and 1.0.
 *
 * @param v0  The lower value used when interpolating.
 * @param v1  The upper value used when interpolating.
 * @param t   The interpolation factor between 0.0 and 1.0.
 * @param v   Pointer to the placeholder for the interpolated value.
 *
 * @return 0 on success, error code on error.
 *
 * @section Notes
 *
 * This can be used for alpha-blending or easing of motion, amongst
 * other things.
 *
 * For example, you can use zsl_interp_lerp to move an object between two points
 * in 5% increments, creating a smooth animation that slows down as you
 * approach the second point. For each 'step' between the two points you can
 * calculate the new position as follows:
 *
 * @code
 * zsl_real_t x, y;
 * x = y = 0.0;
 * // Increment x 5% towards 1.0 (new value = 0.05)
 * zsl_interp_lerp(x, 1.0, 0.05, &x);
 * // Increment y 5% towards 1.5 (new value = 0.075)
 * izsl_nterp_lerp(y, 1.5, 0.05, &y);
 * @endcode
 */
int zsl_interp_lerp(zsl_real_t v0, zsl_real_t v1, zsl_real_t t, zsl_real_t *v);

/**
 * @brief Uses bisection to search the zsl_interp_xy array for the closest array
 * position to 'x', assigning the matching index value to the 'idx' pointer.
 * The array must be monotonic, but can be either increasing or decreasing
 * between xy[0] and xy[n].
 *
 * @param xy  The array of zsl_real_t-based X,Y values to search.
 * @param n   The number of elements in the X,Y array.
 * @param x   The x value to search for.
 * @param idx Pointer to the placeholder for the position of x in the X,Y array.
 *
 * @return 0 on success, error code on error.
 */
int zsl_interp_find_x(struct zsl_interp_xy xy[], size_t n, zsl_real_t x,
		      int *idx);

/**
 * @brief Nearest neighbour (AKA 'piecewise constant') interpolation based on
 * zsl_real_ts.
 *
 * @param xy1 The lower XY pair used when interpolating.
 * @param xy3 The upper XY pair used when interpolating.
 * @param x2  The X value to interpolate for (>= x1 && <= x3).
 * @param y2  Pointer to the placeholder for the interpolated Y value.
 *
 * @return 0 on success, error code on error.
 */
int zsl_interp_nn(struct zsl_interp_xy *xy1, struct zsl_interp_xy *xy3,
		  zsl_real_t x2, zsl_real_t *y2);

/**
 * @brief Nearest neighbour (AKA 'piecewise constant') interpolation based on
 * an array of zsl_real_ts.
 *
 * @param xy  The array of XY pairs tp use when interpolating (min two!).
 * @param n   The number of elements in the XY array.
 * @param x   The X value to interpolate for (x >= xy[0].x, <= xy[n-1].x).
 * @param y   Pointer to the placeholder for the interpolated Y value.
 *
 * @return 0 on success, error code on error.
 */
int zsl_interp_nn_arr(struct zsl_interp_xy xy[], size_t n, zsl_real_t x,
		      zsl_real_t *y);

/**
 * @brief Linear (AKA 'piecewise linear') interpolation for Y between two
 * points, based on zsl_real_ts.
 *
 * @param xy1 The lower XY pair used when interpolating.
 * @param xy3 The upper XY pair used when interpolating.
 * @param x2  The X value to interpolate for (>= x1 && <= x3).
 * @param y2  Pointer to the placeholder for the interpolated Y value.
 *
 * @return 0 on success, error code on error.
 */
int zsl_interp_lin_y(struct zsl_interp_xy *xy1, struct zsl_interp_xy *xy3,
		     zsl_real_t x2, zsl_real_t *y2);

/**
 * @brief Linear (AKA 'piecewise linear') interpolation for Y between two
 * points, based on zsl_real_ts.
 *
 * @param xy  The array of XY pairs tp use when interpolating (min two!).
 * @param n   The number of elements in the XY array.
 * @param x   The X value to interpolate for (x >= xy[0].x, <= xy[n-1].x).
 * @param y   Pointer to the placeholder for the interpolated Y value.
 *
 * @return 0 on success, error code on error.
 */
int zsl_interp_lin_y_arr(struct zsl_interp_xy xy[], size_t n, zsl_real_t x,
			 zsl_real_t *y);

/**
 * @brief Linear (AKA 'piecewise linear') interpolation for X between two
 * points, based on zsl_real_ts.
 *
 * @param xy1 The lower XY pair used when interpolating.
 * @param xy3 The upper XY pair used when interpolating.
 * @param y2  The Y value to interpolate for (>= y1 && <= y3).
 * @param x2  Pointer to the placeholder for the interpolated X value.
 *
 * @return 0 on success, error code on error.
 */
int zsl_interp_lin_x(struct zsl_interp_xy *xy1, struct zsl_interp_xy *xy3,
		     zsl_real_t y2, zsl_real_t *x2);

/**
 * @brief Calculates xyc[n].y2 for natural cubic spline interpolation, based
 *        on the assigned xyc[n].x and xyc[n].y values.
 *
 * @param xyc The array of X,Y,Y2 values to use when interpolating (min four!).
 * @param n   The number of elements in the X,Y,Y2 array.
 * @param yp1 1st derivative at 1. Set to >= 1e30 for natural spline.
 * @param ypn 1st derivative at n'th point. Set to >= 1e30 for natural spline.
 *
 * NOTE: This function must be called BEFORE using zsl_interp_cubic_arr.
 */
int zsl_interp_cubic_calc(struct zsl_interp_xyc xyc[], size_t n,
			  zsl_real_t yp1, zsl_real_t ypn);

/**
 * @brief Natural cubic spline interpolation between two points, based on
 *        zsl_real_ts.
 *
 * @param xyc The array of X,Y,Y2 values to use when interpolating (min four!).
 * @param n   The number of elements in the X,Y,Y2 array.
 * @param x   The X value to interpolate for (x >= xyc[1].x, <= xyc[n-3].x).
 * @param y   Pointer to the placeholder for the interpolated Y value.
 *
 * @return 0 on success, error code on error.
 */
int zsl_interp_cubic_arr(struct zsl_interp_xyc xyc[], size_t n,
			 zsl_real_t x, zsl_real_t *y);

/** @} */ /* End of INTERP_FUNCS group */

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_INTERP_H_ */

/** @} */ /* End of interpolation group */
