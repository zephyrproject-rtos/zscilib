/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>
#include <zsl/zsl.h>
#include <zsl/matrices.h>
#include <zsl/vectors.h>
#include <zsl/physics/momentum.h>
#include "floatcheck.h"

ZTEST(zsl_tests, test_phy_mom_mom)
{
	int rc;
	zsl_real_t p;

	rc = zsl_phy_mom_mom(15.0, 5.31, &p);
	zassert_true(rc == 0, NULL);
 	zassert_true(val_is_equal(p, 79.65, 1E-6), NULL);

	/* Example for negative mass. */
	rc = zsl_phy_mom_mom(-15.0, 5.31, &p);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(p != p, NULL);
}

ZTEST(zsl_tests, test_phy_mom_imp)
{
	int rc;
	zsl_real_t imp;

	rc = zsl_phy_mom_imp(50.0, 3.0, &imp);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(imp, 150.0, 1E-6), NULL);

	/* Example for negative time. */
	rc = zsl_phy_mom_imp(50.0, -3.0, &imp);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(imp != imp, NULL);
}

ZTEST(zsl_tests, test_phy_mom_change)
{
	int rc;
	zsl_real_t p;

	rc = zsl_phy_mom_change(53.2, 15.0, 12.0, &p);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(p, -159.6, 1E-6), NULL);

	/* Example for negative mass. */
	rc = zsl_phy_mom_change(-53.2, 15.0, 12.0, &p);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(p != p, NULL);
}

ZTEST(zsl_tests, test_phy_mom_elas)
{
	int rc;
	zsl_real_t vf2;

	rc = zsl_phy_mom_elas(13.1, 15.6, 9.6, 11.1, -4.3, &vf2);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(vf2, 2.7810810811, 1E-6), NULL);

	/* Example for negative mass 'm1'. */
	rc = zsl_phy_mom_elas(-13.1, 15.6, 9.6, 11.1, -4.3, &vf2);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(vf2 != vf2, NULL);
	
	/* Example for zero mass 'm2'. */
	rc = zsl_phy_mom_elas(13.1, 15.6, 9.6, 0.0, -4.3, &vf2);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(vf2 != vf2, NULL);
	
	/* Example for negative mass 'm2'. */
	rc = zsl_phy_mom_elas(13.1, 15.6, 9.6, -11.1, -4.3, &vf2);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(vf2 != vf2, NULL);
}

ZTEST(zsl_tests, test_phy_mom_inelas)
{
	int rc;
	zsl_real_t vf;

	rc = zsl_phy_mom_inelas(13.1, 15.6, 11.1, -4.3, &vf);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(vf, 6.4723140496, 1E-6), NULL);

	/* Example for negative mass 'm1'. */
	rc = zsl_phy_mom_inelas(-13.1, 15.6, 11.1, -4.3, &vf);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(vf != vf, NULL);
	
	/* Example for negative mass 'm2'. */
	rc = zsl_phy_mom_inelas(13.1, 15.6, -11.1, -4.3, &vf);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(vf != vf, NULL);
	
	/* Example for (m1 + m2) = 0 kg. */
	rc = zsl_phy_mom_inelas(0.0, 15.6, 0.0, -4.3, &vf);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(vf != vf, NULL);
}
