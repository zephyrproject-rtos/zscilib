/*
 * Copyright (c) 2021 Kevin Townsend
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>
#include <zsl/zsl.h>
#include <zsl/orientation/compass.h>
#include "floatcheck.h"

ZTEST(zsl_tests, test_comp_dms_to_dd)
{
	int rc = 0;

	zsl_real_t dd;

	/* Convert the minutes and seconds to degrees. */
	rc = zsl_comp_dms_to_dd(2.0, 29.5, 30.0, &dd);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(dd, 2.5, 1E-6), NULL);
	rc = zsl_comp_dms_to_dd(-2.0, 29.5, 30.0, &dd);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(dd, -2.5, 1E-6), NULL);

	/* Special cases where the inputs are invalid. */
	rc = zsl_comp_dms_to_dd(-190.0, 29.5, 30.0, &dd);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_comp_dms_to_dd(200.0, 29.5, 30.0, &dd);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_comp_dms_to_dd(2.0, 67.5, 30.0, &dd);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_comp_dms_to_dd(2.0, -5.0, 30.0, &dd);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_comp_dms_to_dd(2.0, 29.5, 100.0, &dd);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_comp_dms_to_dd(2.0, 29.5, -25.0, &dd);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_comp_magn_north)
{
	int rc = 0;

	ZSL_VECTOR_DEF(m, 3);
	ZSL_VECTOR_DEF(m2, 4);

	m.data[0] = 1.3;
	m.data[1] = 0.0;
	m.data[2] = 3.0;

	zsl_real_t d;

	/* Calculate the magnetic North. */
	rc = zsl_comp_magn_north(&m, &d);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(d, 0.0, 1E-6), NULL);

	m.data[0] = -1.3;
	m.data[1] = 0.0;
	m.data[2] = 3.0;
	rc = zsl_comp_magn_north(&m, &d);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(d, 180.0, 1E-6), NULL);

	m.data[0] = 1.3;
	m.data[1] = 1.3;
	m.data[2] = 3.0;
	rc = zsl_comp_magn_north(&m, &d);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(d, 45.0, 1E-6), NULL);

	/* Special case where the input is invalid. */
	rc = zsl_comp_magn_north(&m2, &d);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_comp_geo_north)
{
	int rc = 0;

	ZSL_VECTOR_DEF(m, 3);
	ZSL_VECTOR_DEF(m2, 4);

	m.data[0] = 1.3;
	m.data[1] = 0.0;
	m.data[2] = 3.0;

	zsl_real_t d;

	/* Calculate the geographic North. */
	rc = zsl_comp_geo_north(&m, 10.0, &d);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(d, 10.0, 1E-6), NULL);

	m.data[0] = -1.3;
	m.data[1] = 0.0;
	m.data[2] = 3.0;
	rc = zsl_comp_geo_north(&m, 10.0, &d);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(d, 190.0, 1E-6), NULL);

	m.data[0] = 1.3;
	m.data[1] = 1.3;
	m.data[2] = 3.0;
	rc = zsl_comp_geo_north(&m, 10.0, &d);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(d, 55.0, 1E-6), NULL);

	/* Special case where the input is invalid. */
	rc = zsl_comp_geo_north(&m2, 10.0, &d);
	zassert_true(rc == -EINVAL, NULL);
}
