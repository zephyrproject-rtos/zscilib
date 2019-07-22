/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ztest.h>
#include <zsl/zsl.h>
#include <zsl/matrices.h>
#include <zsl/vectors.h>
#include <zsl/physics/eleccomp.h>
#include "floatcheck.h"

void test_phy_ecmp_capac_cpv(void)
{
	int rc;
	zsl_real_t c;

	rc = zsl_phy_ecmp_capac_cpv(100.0, 10.0, &c);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(c, 10.0, 1E-6), NULL);

	rc = zsl_phy_ecmp_capac_cpv(100.0, 0.0, &c);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(c != c, NULL);
}

void test_phy_ecmp_capac_ad(void)
{
	int rc;
	zsl_real_t c;

	/* E * 0.015 m^2 plate / 0.006 m gap = 22.135 pF */
	rc = zsl_phy_ecmp_capac_ad(150E-4, 0.6E-2, &c);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(c, 2.2135E-11, 1E-13), NULL);

	rc = zsl_phy_ecmp_capac_ad(150E-4, 0.0, &c);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(c != c, NULL);
}
