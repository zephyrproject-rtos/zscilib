/*
 * Copyright (c) 2019-2020 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>
#include <zephyr/sys/printk.h>
#include <zsl/colorimetry.h>
#include "floatcheck.h"
#include "data.h"

ZTEST(zsl_tests, test_conv_spd_xyz)
{
	int rc;
	struct zsl_clr_xyz xyz;

	rc = zsl_clr_conv_spd_xyz(&zsl_clr_test_spd_5983k, ZSL_CLR_OBS_2_DEG, &xyz);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(xyz.xyz_x, 0.9516492, 1E-5), NULL);
	zassert_true(val_is_equal(xyz.xyz_y, 1.0, 1E-5), NULL);
	zassert_true(val_is_equal(xyz.xyz_z, 1.0056107, 1E-5), NULL);
	zassert_true(xyz.observer == ZSL_CLR_OBS_2_DEG, NULL);
	zassert_false(xyz.x_invalid, NULL);
	zassert_false(xyz.y_invalid, NULL);
	zassert_false(xyz.z_invalid, NULL);
}

ZTEST(zsl_tests, test_conv_ct_xyz)
{
	int rc;
	struct zsl_clr_xyz xyz;

	memset(&xyz, 0, sizeof xyz);

	rc = zsl_clr_conv_ct_xyz(5600.0, ZSL_CLR_OBS_2_DEG, &xyz);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(xyz.xyz_x, 0.9738203, 1E-6), NULL);
	zassert_true(val_is_equal(xyz.xyz_y, 1.0, 1E-6), NULL);
	zassert_true(val_is_equal(xyz.xyz_z, 0.9751908, 1E-6), NULL);
	zassert_true(xyz.observer == ZSL_CLR_OBS_2_DEG, NULL);
	zassert_false(xyz.x_invalid, NULL);
	zassert_false(xyz.y_invalid, NULL);
	zassert_false(xyz.z_invalid, NULL);
}

ZTEST(zsl_tests, test_conv_ct_rgb8)
{
	int rc;
	struct zsl_clr_rgb8 rgb;
	struct zsl_mtx *srgb_ccm;

	/* Get sRGB D65 correlation matrix for testing. */
	zsl_clr_rgbccm_get(ZSL_CLR_RGB_CCM_SRGB_D65, &srgb_ccm);

	memset(&rgb, 0, sizeof rgb);

	rc = zsl_clr_conv_ct_rgb8(5600.0, ZSL_CLR_OBS_2_DEG, srgb_ccm, &rgb);
	zassert_true(rc == 0, NULL);
	zassert_true(rgb.r == 255, NULL);
	zassert_true(rgb.g == 248, NULL);
	zassert_true(rgb.b == 224, NULL);
	zassert_true(rgb.a == 255, NULL);
	zassert_true(rgb.r_invalid, NULL);  /* R is out of gamut! */
	zassert_false(rgb.g_invalid, NULL);
	zassert_false(rgb.b_invalid, NULL);
	zassert_false(rgb.a_invalid, NULL);
}

ZTEST(zsl_tests, test_conv_cct_xyy)
{
	int rc;
	struct zsl_clr_cct cct;
	struct zsl_clr_xyy xyy;

	memset(&cct, 0, sizeof cct);
	memset(&xyy, 0, sizeof xyy);

	/* Test 1: Set the test CCT to 2900K (Duv 0.0) */
	cct.cct = 2900.0;
	cct.duv = 0.0;

	rc = zsl_clr_conv_cct_xyy(&cct, ZSL_CLR_OBS_2_DEG, &xyy);
	zassert_true(rc == 0, NULL);

	/* Check the xyY output to the sixth decimal place. */
	/* 2900 CCT = xyY (0.4442166, 0.4064293, 1.000000) w/5 nm lookup. */
	zassert_true(val_is_equal(xyy.xyy_x, 0.4442166, 1E-6), NULL);
	zassert_true(val_is_equal(xyy.xyy_y, 0.4064293, 1E-6), NULL);
	zassert_true(xyy.xyy_Y == 1.0, NULL);
	zassert_true(xyy.observer == ZSL_CLR_OBS_2_DEG, NULL);
	zassert_false(xyy.x_invalid, NULL);
	zassert_false(xyy.y_invalid, NULL);
	zassert_false(xyy.Y_invalid, NULL);

	/* Test 2: Set the test CCT to 2938K with a Duv of -0.00082 */
	cct.cct = 2938.0;
	cct.duv = -0.00082;

	rc = zsl_clr_conv_cct_xyy(&cct, ZSL_CLR_OBS_2_DEG, &xyy);
	zassert_true(rc == 0, NULL);

	/* Check the xyY output to the third or sixth decimal place.
	 *
	 * With a precision lookup (calculated externally) we get:
	 *
	 *     xyY (0.4401769103, 0.4030829267, 1.000000)
	 */
#if CONFIG_ZSL_SINGLE_PRECISION
	zassert_true(val_is_equal(xyy.xyy_x, 0.4399290, 1E-3), NULL);
	zassert_true(val_is_equal(xyy.xyy_y, 0.4030270, 1E-3), NULL);
#else
	zassert_true(val_is_equal(xyy.xyy_x, 0.4401715, 1E-6), NULL);
	zassert_true(val_is_equal(xyy.xyy_y, 0.4030739, 1E-6), NULL);
#endif
	zassert_true(xyy.xyy_Y == 1.0, NULL);
	zassert_true(xyy.observer == ZSL_CLR_OBS_2_DEG, NULL);
	zassert_false(xyy.x_invalid, NULL);
	zassert_false(xyy.y_invalid, NULL);
	zassert_false(xyy.Y_invalid, NULL);
}

ZTEST(zsl_tests, test_conv_cct_xyz)
{
	int rc;
	struct zsl_clr_cct cct;
	struct zsl_clr_xyz xyz;

	memset(&cct, 0, sizeof cct);
	memset(&xyz, 0, sizeof xyz);

	/* Test 1: Set the test CCT to 2900K (Duv 0.0) */
	cct.cct = 2900.0;
	cct.duv = 0.0;

	rc = zsl_clr_conv_cct_xyz(&cct, ZSL_CLR_OBS_2_DEG, &xyz);
	zassert_true(rc == 0, NULL);

	/* Check the XYZ output to the sixth decimal place. */
	/* 2900K CCT = XYZ (1.0929741, 1.0, 0.3674785) w/5nm lookup. */
	zassert_true(val_is_equal(xyz.xyz_x, 1.0929741, 1E-6), NULL);
	zassert_true(val_is_equal(xyz.xyz_y, 1.00000000, 1E-6), NULL);
	zassert_true(val_is_equal(xyz.xyz_z, 0.3674785, 1E-6), NULL);
	zassert_true(xyz.observer == ZSL_CLR_OBS_2_DEG, NULL);
	zassert_false(xyz.x_invalid, NULL);
	zassert_false(xyz.y_invalid, NULL);
	zassert_false(xyz.z_invalid, NULL);

	/* Test 2: Set the test CCT to 2938K with a Duv of -0.00082 */
	cct.cct = 2938.0;
	cct.duv = -0.00082;

	rc = zsl_clr_conv_cct_xyz(&cct, ZSL_CLR_OBS_2_DEG, &xyz);
	zassert_true(rc == 0, NULL);

	/* Check the XYZ output to the third or fourth decimal place.
	 *
	 * With a precision lookup (calculated externally) we get:
	 *
	 *     XYZ (1.092025687995, 1.0, 0.388853391245)
	 */
#if CONFIG_ZSL_SINGLE_PRECISION
	zassert_true(val_is_equal(xyz.xyz_x, 1.092025687995, 1E-3), NULL);
	zassert_true(val_is_equal(xyz.xyz_y, 1.00000000, 1E-3), NULL);
	zassert_true(val_is_equal(xyz.xyz_z, 0.388853391245, 1E-3), NULL);
#else
	/* TODO: Figure out why only accurate to four places? :( */
	zassert_true(val_is_equal(xyz.xyz_x, 1.092025687995, 1E-4), NULL);
	zassert_true(val_is_equal(xyz.xyz_y, 1.00000000, 1E-4), NULL);
	zassert_true(val_is_equal(xyz.xyz_z, 0.388853391245, 1E-4), NULL);
#endif
	zassert_true(xyz.observer == ZSL_CLR_OBS_2_DEG, NULL);
	zassert_false(xyz.x_invalid, NULL);
	zassert_false(xyz.y_invalid, NULL);
	zassert_false(xyz.z_invalid, NULL);
}

ZTEST(zsl_tests, test_conv_uv60_cct_ohno2011)
{
	int rc;
	struct zsl_clr_uv60 uv;
	struct zsl_clr_cct cct;
	zsl_real_t eps_duv;
	zsl_real_t eps_cct;

	/* Set the test tolerances. */
	eps_duv = 0.001;
	eps_cct = 5.0;

	/* 2011K in uv. */
	memset(&cct, 0, sizeof cct);
	memset(&uv, 0, sizeof uv);
	uv.uv60_u = 0.30412;
	uv.uv60_v = 0.35898;
	rc = zsl_clr_conv_uv60_cct(ZSL_CLR_UV_CCT_METHOD_OHNO2011, &uv, &cct);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(cct.duv, 0.0, eps_duv), NULL);
	zassert_true(val_is_equal(cct.cct, 2011.0, eps_cct), NULL);

	/* 2900K + Duv 0.02 in uv. */
	memset(&cct, 0, sizeof cct);
	uv.uv60_u = 0.247629;
	uv.uv60_v = 0.367808;
	rc = zsl_clr_conv_uv60_cct(ZSL_CLR_UV_CCT_METHOD_OHNO2011, &uv, &cct);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(cct.duv, 0.02, eps_duv), NULL);
	zassert_true(val_is_equal(cct.cct, 2900.0, eps_cct), NULL);

	/* 7057.7 in uv. */
	memset(&cct, 0, sizeof cct);
	uv.uv60_u = 0.197888;
	uv.uv60_v = 0.306665;
	rc = zsl_clr_conv_uv60_cct(ZSL_CLR_UV_CCT_METHOD_OHNO2011, &uv, &cct);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(cct.duv, 0.0, eps_duv), NULL);
	zassert_true(val_is_equal(cct.cct, 7057.7, eps_cct), NULL);
}

ZTEST(zsl_tests, test_conv_uv60_cct_ohno2014)
{
	int rc;
	struct zsl_clr_uv60 uv;
	struct zsl_clr_cct cct;
	zsl_real_t eps_duv;
	zsl_real_t eps_cct;

	/* Set the test tolerances. */
	eps_duv = 0.0001;
	eps_cct = 10.0;

	/* Test 1: 2938 K with a Duv of -0.00082 */
	memset(&cct, 0, sizeof cct);
	memset(&uv, 0, sizeof uv);
	uv.uv60_u = 0.25309737;
	uv.uv60_v = 0.34765305;
	rc = zsl_clr_conv_uv60_cct(ZSL_CLR_UV_CCT_METHOD_OHNO2014, &uv, &cct);
	zassert_true(rc == 0, NULL);
	printk("%d %d\n", (int)cct.cct, (int)(cct.duv * 1000000000));
	zassert_true(val_is_equal(cct.duv, -0.00082, eps_duv), NULL);
	zassert_true(val_is_equal(cct.cct, 2938.0, eps_cct), NULL);

	/* Test 2: 7243.94 K with a Duv of 0.0. */
	memset(&cct, 0, sizeof cct);
	memset(&uv, 0, sizeof uv);
	uv.uv60_u = 0.197153338321965;
	uv.uv60_v = 0.305540116604965;
	rc = zsl_clr_conv_uv60_cct(ZSL_CLR_UV_CCT_METHOD_OHNO2014, &uv, &cct);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(cct.duv, 0.0, eps_duv), NULL);
	zassert_true(val_is_equal(cct.cct, 7243.94, eps_cct), NULL);

	/* TODO: Add further tests! */
}
