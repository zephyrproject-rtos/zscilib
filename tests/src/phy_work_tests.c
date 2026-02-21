/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>
#include <zsl/zsl.h>
#include <zsl/matrices.h>
#include <zsl/vectors.h>
#include <zsl/physics/work.h>
#include "floatcheck.h"

ZTEST(zsl_tests, test_phy_work_module)
{
	int rc;
	zsl_real_t w;

	rc = zsl_phy_work_module(54.2, 3.0, &w);
	zassert_true(rc == 0);
	zassert_true(val_is_equal(w, 162.6, 1E-6));

	/* Example for negative force. */
	rc = zsl_phy_work_module(-54.2, 3.0, &w);
	zassert_true(rc == -EINVAL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(w != w);

	/* Example for negative distance. */
	rc = zsl_phy_work_module(54.2, -3.0, &w);
	zassert_true(rc == -EINVAL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(w != w);
}

ZTEST(zsl_tests, test_phy_work_x)
{
	int rc;
	zsl_real_t w;

	rc = zsl_phy_work_x(54.2, 3.0, 0.4, &w);
	zassert_true(rc == 0);
#ifdef CONFIG_ZSL_SINGLE_PRECISION
 	zassert_true(val_is_equal(w, 149.7645176249, 1E-4));
#else
	zassert_true(val_is_equal(w, 149.7645176249, 1E-8));
#endif

	/* Example for negative force. */
	rc = zsl_phy_work_x(-54.2, 3.0, 0.4, &w);
	zassert_true(rc == -EINVAL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(w != w);

	/* Example for negative distance. */
	rc = zsl_phy_work_x(54.2, -3.0, 0.4, &w);
	zassert_true(rc == -EINVAL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(w != w);
}

ZTEST(zsl_tests, test_phy_work_y)
{
	int rc;
	zsl_real_t w;

	rc = zsl_phy_work_y(54.2, 3.0, 0.4, &w);
	zassert_true(rc == 0);
	zassert_true(val_is_equal(w, 63.3194224594, 1E-6));

	/* Example for negative force. */
	rc = zsl_phy_work_y(-54.2, 3.0, 0.4, &w);
	zassert_true(rc == -EINVAL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(w != w);

	/* Example for negative distance. */
	rc = zsl_phy_work_y(54.2, -3.0, 0.4, &w);
	zassert_true(rc == -EINVAL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(w != w);
}

ZTEST(zsl_tests, test_phy_work_kin)
{
	int rc;
	zsl_real_t w;

	rc = zsl_phy_work_kin(14.0, 35.0, &w);
	zassert_true(rc == 0);
	zassert_true(val_is_equal(w, 21.0, 1E-6));
}
