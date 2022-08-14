/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>
#include <zsl/zsl.h>
#include <zsl/interp.h>
#include "floatcheck.h"

void test_interp_lerp(void)
{
	int rc;
	zsl_real_t v0, v1, v;
	zsl_real_t t;

	v0 = 0.1f;
	v1 = 0.2f;
	t = 0.5f;

	/* Test 1: Normal conversion. */
	rc = zsl_interp_lerp(v0, v1, t, &v);
	zassert_equal(rc, 0, NULL);
	zassert_true(val_is_equal(v, 0.15, 1E-4), NULL);

	/* Test 2: t = 0. */
	t = 0.0f;
	rc = zsl_interp_lerp(v0, v1, t, &v);
	zassert_equal(rc, 0, NULL);
	zassert_true(val_is_equal(v, v0, 1E-4), NULL);

	/* Test 3: t = 1.0. */
	t = 1.0f;
	rc = zsl_interp_lerp(v0, v1, t, &v);
	zassert_equal(rc, 0, NULL);
	zassert_true(val_is_equal(v, v1, 1E-4), NULL);

	/* Test 4: t = -0.01 (invalid value). */
	t = -0.01f;
	rc = zsl_interp_lerp(v0, v1, t, &v);
	zassert_equal(rc, -EINVAL, NULL);

	/* Test 5: t = 1.01 (invalid value). */
	t = 1.01f;
	rc = zsl_interp_lerp(v0, v1, t, &v);
	zassert_equal(rc, -EINVAL, NULL);

}

void test_interp_find_x_asc(void)
{
	int rc;
	int idx;
	unsigned int n;
	zsl_real_t x;
	struct zsl_interp_xy xy[6];

	/* (x:-1, y:-3.0) */
	xy[0].x = -1;
	xy[0].y = -3.0f;

	/* (x:0, y:-2.0) */
	xy[1].x = 0;
	xy[1].y = -2.0f;

	/* (x:1, y:-1.0) */
	xy[2].x = 1;
	xy[2].y = -1.0f;

	/* (x:2, y:0) */
	xy[3].x = 2;
	xy[3].y = 0.0f;

	/* (x:3, y:1.0) */
	xy[4].x = 3;
	xy[4].y = 1.0f;

	/* (x:4, y:2.0) */
	xy[5].x = 4;
	xy[5].y = 2.0f;

	/* Calculate the number of entries in xy. */
	n = sizeof xy / sizeof xy[0];

	/* Test 1: Try to find x = -0.25 (valid range) */
	x = -0.25f;
	rc = zsl_interp_find_x(xy, n, x, &idx);
	zassert_equal(rc, 0, NULL);
	zassert_equal(idx, 0, NULL);

	/* Test 2: Try to find x = 3.5 (valid range) */
	x = 3.5f;
	rc = zsl_interp_find_x(xy, n, x, &idx);
	zassert_equal(rc, 0, NULL);
	zassert_equal(idx, 4, NULL);

	/* Test 3: Try to find x = -1.25 (invalid range on the low end). */
	x = -1.25f;
	rc = zsl_interp_find_x(xy, n, x, &idx);
	zassert_equal(rc, -EINVAL, NULL);
	zassert_equal(idx, -1, NULL);

	/* Test 4: Try to find x = 4.25 (invalid range on the high end). */
	x = 4.25f;
	rc = zsl_interp_find_x(xy, n, x, &idx);
	zassert_equal(rc, -EINVAL, NULL);
	zassert_equal(idx, n, NULL);

	/* Test 5: Try to find x = upper limit. */
	x = xy[n - 1].x;
	rc = zsl_interp_find_x(xy, n, x, &idx);
	zassert_equal(rc, 0, NULL);
	zassert_equal(idx, n - 2, NULL);

	/* Test 6: Try to find x = lower limit. */
	x = xy[0].x;
	rc = zsl_interp_find_x(xy, n, x, &idx);
	zassert_equal(rc, 0, NULL);
	zassert_equal(idx, 0, NULL);

	/* Test 7: Try to find x = upper limit + 0.1 */
	x = xy[n - 1].x + 0.1f;
	rc = zsl_interp_find_x(xy, n, x, &idx);
	zassert_equal(rc, -EINVAL, NULL);
	zassert_equal(idx, n, NULL);

	/* Test 8: Try to find x = lower limit - 0.1 */
	x = xy[0].x - 0.1f;
	rc = zsl_interp_find_x(xy, n, x, &idx);
	zassert_equal(rc, -EINVAL, NULL);
	zassert_equal(idx, -1, NULL);
}

void test_interp_find_x_desc(void)
{
	int rc;
	int idx;
	unsigned int n;
	zsl_real_t x;
	struct zsl_interp_xy xy[6];

	/* (x:4, y:2.0) */
	xy[0].x = 4;
	xy[0].y = 2.0f;

	/* (x:3, y:1.0) */
	xy[1].x = 3;
	xy[1].y = 1.0f;

	/* (x:2, y:0) */
	xy[2].x = 2;
	xy[2].y = 0.0f;

	/* (x:1, y:-1.0) */
	xy[3].x = 1;
	xy[3].y = -1.0f;

	/* (x:0, y:-2.0) */
	xy[4].x = 0;
	xy[4].y = -2.0f;

	/* (x:-1, y:-3.0) */
	xy[5].x = -1;
	xy[5].y = -3.0f;

	/* Calculate the number of entries in xy. */
	n = sizeof xy / sizeof xy[0];

	/* Test 1: Try to find x = -0.25 (valid range) */
	x = -0.25f;
	rc = zsl_interp_find_x(xy, n, x, &idx);
	zassert_equal(rc, 0, NULL);
	zassert_equal(idx, 4, NULL);

	/* Test 2: Try to find x = 3.5 (valid range) */
	x = 3.5f;
	rc = zsl_interp_find_x(xy, n, x, &idx);
	zassert_equal(rc, 0, NULL);
	zassert_equal(idx, 0, NULL);

	/* Test 3: Try to find x = 1.75 (valid range) */
	x = 1.75f;
	rc = zsl_interp_find_x(xy, n, x, &idx);
	zassert_equal(rc, 0, NULL);
	zassert_equal(idx, 2, NULL);

	/* Test 4: Try to find x = -1.25 (invalid range on the high end). */
	x = -1.25f;
	rc = zsl_interp_find_x(xy, n, x, &idx);
	zassert_equal(rc, -EINVAL, NULL);
	zassert_equal(idx, n, NULL);

	/* Test 5: Try to find x = 4.25 (invalid range on the low end). */
	x = 4.25f;
	rc = zsl_interp_find_x(xy, n, x, &idx);
	zassert_equal(rc, -EINVAL, NULL);
	zassert_equal(idx, -1, NULL);

	/* Test 6: Try to find x = upper limit. */
	x = xy[n - 1].x;
	rc = zsl_interp_find_x(xy, n, x, &idx);
	zassert_equal(rc, 0, NULL);
	zassert_equal(idx, n - 2, NULL);

	/* Test 7: Try to find x = lower limit. */
	x = xy[0].x;
	rc = zsl_interp_find_x(xy, n, x, &idx);
	zassert_equal(rc, 0, NULL);
	zassert_equal(idx, 0, NULL);

	/* Test 8: Try to find x = upper limit - 0.1 */
	x = xy[n - 1].x - 0.1f;
	rc = zsl_interp_find_x(xy, n, x, &idx);
	zassert_equal(rc, -EINVAL, NULL);
	zassert_equal(idx, n, NULL);

	/* Test 9: Try to find x = lower limit + 0.1 */
	x = xy[0].x + 0.1f;
	rc = zsl_interp_find_x(xy, n, x, &idx);
	zassert_equal(rc, -EINVAL, NULL);
	zassert_equal(idx, -1, NULL);
}

void test_interp_nn(void)
{
	int rc;
	struct zsl_interp_xy xy1;
	struct zsl_interp_xy xy3;
	zsl_real_t y2;

	xy1.x = 10.0f;
	xy1.y = 0.20f;
	xy3.x = 20.0f;
	xy3.y = 0.22f;

	/* Test 1: Interpolate y at x = 13.6 */
	rc = zsl_interp_nn(&xy1, &xy3, 13.6f, &y2);
	zassert_equal(rc, 0, NULL);
	zassert_true(val_is_equal(y2, xy1.y, 1E-4F), NULL);

	/* Test 2: Interpolate y at x = 15.6 */
	rc = zsl_interp_nn(&xy1, &xy3, 15.6f, &y2);
	zassert_equal(rc, 0, NULL);
	zassert_true(val_is_equal(y2, xy3.y, 1E-4F), NULL);

	/* Test 3: Interpolate y at x = 15.0 */
	rc = zsl_interp_nn(&xy1, &xy3, 15.0f, &y2);
	zassert_equal(rc, 0, NULL);
	zassert_true(val_is_equal(y2, xy3.y, 1E-4F), NULL);

	/* Test 4: Check for delta 0.0 on x. */
	xy3.x = xy1.x;
	rc = zsl_interp_nn(&xy1, &xy3, xy1.x, &y2);
	zassert_equal(rc, -EINVAL, NULL);
	xy3.x = 20.0f;

	/* Test 5: Negative values. */
	xy1.x = -4.5f;
	xy3.x = 10.0f;
	rc = zsl_interp_nn(&xy1, &xy3, 2.75f, &y2);
	zassert_equal(rc, 0, NULL);
	zassert_true(val_is_equal(y2, xy3.y, 1E-4F), NULL);
	xy1.x = 10.0f;
	xy3.x = 20.0f;

	/* Test 6: x2 == x1. */
	rc = zsl_interp_nn(&xy1, &xy3, xy1.x, &y2);
	zassert_equal(rc, 0, NULL);
	zassert_true(val_is_equal(y2, xy1.y, 1E-4F), NULL);

	/* Test 7: x2 == x3. */
	rc = zsl_interp_nn(&xy1, &xy3, xy3.x, &y2);
	zassert_equal(rc, 0, NULL);
	zassert_true(val_is_equal(y2, xy3.y, 1E-4F), NULL);
}

void test_interp_nn_arr(void)
{
	int rc;
	unsigned int n;
	zsl_real_t x;
	zsl_real_t y;
	struct zsl_interp_xy xy[6];

	/* (x:-1, y:-3.0) */
	xy[0].x = -1;
	xy[0].y = -3.0f;

	/* (x:0, y:-2.0) */
	xy[1].x = 0;
	xy[1].y = -2.0f;

	/* (x:1, y:-1.0) */
	xy[2].x = 1;
	xy[2].y = -1.0f;

	/* (x:2, y:0) */
	xy[3].x = 2;
	xy[3].y = 0.0f;

	/* (x:3, y:1.0) */
	xy[4].x = 3;
	xy[4].y = 1.0f;

	/* (x:4, y:2.0) */
	xy[5].x = 4;
	xy[5].y = 2.0f;

	/* Calculate the number of entries in xy. */
	n = sizeof xy / sizeof xy[0];

	/* Test 1: Try to find x = -0.25 (valid range) */
	x = -0.25f;
	rc = zsl_interp_nn_arr(xy, n, x, &y);
	zassert_equal(rc, 0, NULL);
	zassert_true(val_is_equal(y, xy[1].y, 1E-4F), NULL);
}

void test_interp_lin_y(void)
{
	int rc;
	struct zsl_interp_xy xy1;
	struct zsl_interp_xy xy3;
	zsl_real_t y2;

	xy1.x = 10.0f;
	xy1.y = 0.20f;
	xy3.x = 20.0f;
	xy3.y = 0.22f;

	/* Test 1: Interpolate y at x = 13.6 */
	rc = zsl_interp_lin_y(&xy1, &xy3, 13.6f, &y2);
	zassert_equal(rc, 0, NULL);
	zassert_true(val_is_equal(y2, 0.2072f, 1E-4F), NULL);

	/* Test 2: Check for x too low. */
	rc = zsl_interp_lin_y(&xy1, &xy3, xy1.x - 0.01f, &y2);
	zassert_equal(rc, -EINVAL, NULL);

	/* Test 3: Check for x too high. */
	rc = zsl_interp_lin_y(&xy1, &xy3, xy3.x + 0.01f, &y2);
	zassert_equal(rc, -EINVAL, NULL);

	/* Test 4: Check for delta 0.0 on x. */
	xy3.x = xy1.x;
	rc = zsl_interp_lin_y(&xy1, &xy3, xy1.x, &y2);
	zassert_equal(rc, -EINVAL, NULL);
	xy3.x = 20.0f;

	/* Test 5: Negative values. */
	xy1.x = -4.5f;
	xy3.x = 10.0f;
	rc = zsl_interp_lin_y(&xy1, &xy3, 2.75f, &y2);
	zassert_equal(rc, 0, NULL);
	zassert_true(val_is_equal(y2, 0.21f, 1E-4F), NULL);
	xy1.x = 10.0f;
	xy3.x = 20.0f;

	/* Test 6: x2 == x1. */
	rc = zsl_interp_lin_y(&xy1, &xy3, xy1.x, &y2);
	zassert_equal(rc, 0, NULL);
	zassert_true(val_is_equal(y2, xy1.y, 1E-4F), NULL);

	/* Test 7: x2 == x3. */
	rc = zsl_interp_lin_y(&xy1, &xy3, xy3.x, &y2);
	zassert_equal(rc, 0, NULL);
	zassert_true(val_is_equal(y2, xy3.y, 1E-4F), NULL);
}

void test_interp_lin_y_arr(void)
{
	int rc;
	unsigned int n;
	zsl_real_t x;
	zsl_real_t y;
	struct zsl_interp_xy xy[6];

	/* (x:-1, y:-3.0) */
	xy[0].x = -1;
	xy[0].y = -3.0f;

	/* (x:0, y:-2.0) */
	xy[1].x = 0;
	xy[1].y = -2.0f;

	/* (x:1, y:-1.0) */
	xy[2].x = 1;
	xy[2].y = -1.0f;

	/* (x:2, y:0) */
	xy[3].x = 2;
	xy[3].y = 0.0f;

	/* (x:3, y:1.0) */
	xy[4].x = 3;
	xy[4].y = 1.0f;

	/* (x:4, y:2.0) */
	xy[5].x = 4;
	xy[5].y = 2.0f;

	/* Calculate the number of entries in xy. */
	n = sizeof xy / sizeof xy[0];

	/* Test 1: Try to find x = -0.25 (valid range) */
	x = -0.25f;
	rc = zsl_interp_lin_y_arr(xy, n, x, &y);
	zassert_equal(rc, 0, NULL);
	zassert_true(val_is_equal(y, -2.25f, 1E-4F), NULL);
}

void test_interp_lin_x(void)
{
	int rc;
	struct zsl_interp_xy xy1;
	struct zsl_interp_xy xy3;
	zsl_real_t x2;

	xy1.x = 10.0f;
	xy1.y = 0.20f;
	xy3.x = 20.0f;
	xy3.y = 0.22f;

	/* Test 1: Interpolate x for x = 0.2072f */
	rc = zsl_interp_lin_x(&xy1, &xy3, 0.2072f, &x2);
	zassert_equal(rc, 0, NULL);
	zassert_true(val_is_equal(x2, 13.6f, 1E-4F), NULL);

	/* Test 2: Check for y too low. */
	rc = zsl_interp_lin_y(&xy1, &xy3, xy1.y - 0.01f, &x2);
	zassert_equal(rc, -EINVAL, NULL);

	/* Test 3: Check for y too high. */
	rc = zsl_interp_lin_y(&xy1, &xy3, xy3.y + 0.01f, &x2);
	zassert_equal(rc, -EINVAL, NULL);

	/* Test 4: Check for delta 0.0 on x. */
	xy3.x = xy1.x;
	rc = zsl_interp_lin_x(&xy1, &xy3, xy1.y, &x2);
	zassert_equal(rc, -EINVAL, NULL);
	xy3.x = 20.0f;

	/* Test 5: Negative values. */
	xy1.y = -0.20f;
	xy3.y = 0.20f;
	rc = zsl_interp_lin_x(&xy1, &xy3, -0.1f, &x2);
	zassert_equal(rc, 0, NULL);
	zassert_true(val_is_equal(x2, 12.5f, 1E-4F), NULL);
	xy1.y = 0.20f;
	xy3.y = 0.22f;

	/* Test 6: y2 == y1. */
	rc = zsl_interp_lin_x(&xy1, &xy3, xy1.y, &x2);
	zassert_equal(rc, 0, NULL);
	zassert_true(val_is_equal(x2, xy1.x, 1E-4F), NULL);

	/* Test 7: y2 == y3. */
	rc = zsl_interp_lin_x(&xy1, &xy3, xy3.y, &x2);
	zassert_equal(rc, 0, NULL);
	zassert_true(val_is_equal(x2, xy3.x, 1E-4F), NULL);
}

void test_interp_cubic_arr(void)
{
	int rc;
	unsigned int n;
	zsl_real_t x;
	zsl_real_t y;
	struct zsl_interp_xyc xyc[7];

	/* Make sure all Y2 values are set to 0 by default. */
	memset(xyc, 0, sizeof xyc);

	xyc[0].x = -3;
	xyc[0].y = 0.0f;
	xyc[1].x = -2;
	xyc[1].y = 1.0f;
	xyc[2].x = -1;
	xyc[2].y = 2.0f;
	xyc[3].x = 0;
	xyc[3].y = .75f;
	xyc[4].x = 1;
	xyc[4].y = 0.0f;
	xyc[5].x = 2;
	xyc[5].y = 2.5f;
	xyc[6].x = 3;
	xyc[6].y = -1.25f;

	/* Calculate the number of entries in xy. */
	n = sizeof xyc / sizeof xyc[0];

	/* Calculate y2 values for xyc. */
	rc = zsl_interp_cubic_calc(xyc, n, 1e30, 1e30);
	zassert_equal(rc, 0, NULL);

	/* Test 1: Try to find x = -1.25 (valid range) */
	x = -1.25f;
	rc = zsl_interp_cubic_arr(xyc, n, x, &y);
	zassert_equal(rc, 0, NULL);
	zassert_true(val_is_equal(y, 1.907918f, 1E-4F), NULL);

	/* Test 2: Not enough samples. */
	x = 0.7f;
	rc = zsl_interp_cubic_arr(xyc, 2, x, &y);
	zassert_equal(rc, -EINVAL, NULL);
}
