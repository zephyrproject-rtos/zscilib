/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>
#include <zsl/zsl.h>
#include <zsl/matrices.h>
#include <zsl/vectors.h>
#include <zsl/physics/electric.h>
#include "floatcheck.h"

ZTEST(zsl_tests, test_phy_elec_charge_dens)
{
	int rc;
	zsl_real_t d;

	rc = zsl_phy_elec_charge_dens(1.2, 2.0, &d);
	zassert_true(rc == 0);
	zassert_true(val_is_equal(d, 0.6, 1E-6));

	/* Example for negative volume. */
	rc = zsl_phy_elec_charge_dens(1.2, -2.0, &d);
	zassert_true(rc == -EINVAL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(d != d);

	/* Example for zero volume. */
	rc = zsl_phy_elec_charge_dens(1.2, 0.0, &d);
	zassert_true(rc == -EINVAL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(d != d);
}

ZTEST(zsl_tests, test_phy_elec_force)
{
	int rc;
	zsl_real_t f;

	rc = zsl_phy_elec_force(2.1E-7, -4.9E-7, 0.05, &f);
	zassert_true(rc == 0);
	zassert_true(val_is_equal(f, -0.3699276316, 1E-6));

	/* Example for negative distance. */
	rc = zsl_phy_elec_force(2.1E-7, -4.9E-7, -0.05, &f);
	zassert_true(rc == -EINVAL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(f != f);

	/* Example for zero distance. */
	rc = zsl_phy_elec_force(2.1E-7, -4.9E-7, 0.0, &f);
	zassert_true(rc == -EINVAL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(f != f);
}

ZTEST(zsl_tests, test_phy_elec_force2)
{
	int rc;
	zsl_real_t f;

	rc = zsl_phy_elec_force2(3.1, -5.4, &f);
	zassert_true(rc == 0);
	zassert_true(val_is_equal(f, -16.74, 1E-6));
}

ZTEST(zsl_tests, test_phy_elec_field)
{
	int rc;
	zsl_real_t e;

	rc = zsl_phy_elec_field(2.1E-9, 0.5, &e);
	zassert_true(rc == 0);
#ifdef CONFIG_ZSL_SINGLE_PRECISION
 	zassert_true(val_is_equal(e, 75.4954350108, 1E-5));
#else
	zassert_true(val_is_equal(e, 75.4954350108, 1E-8));
#endif

	/* Example for negative distance. */
	rc = zsl_phy_elec_field(2.1E-9, -0.5, &e);
	zassert_true(rc == -EINVAL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(e != e);

	/* Example for zero distance. */
	rc = zsl_phy_elec_field(2.1E-9, 0.0, &e);
	zassert_true(rc == -EINVAL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(e != e);
}

ZTEST(zsl_tests, test_phy_elec_pot_ener)
{
	int rc;
	zsl_real_t u;

	rc = zsl_phy_elec_pot_ener(2.1E-7, -4.9E-7, 0.0005, &u);
	zassert_true(rc == 0);
	zassert_true(val_is_equal(u, -1.8496381578, 1E-6));

	/* Example for negative distance. */
	rc = zsl_phy_elec_pot_ener(2.1E-7, -4.9E-7, -0.0005, &u);
	zassert_true(rc == -EINVAL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(u != u);

	/* Example for zero distance. */
	rc = zsl_phy_elec_pot_ener(2.1E-7, -4.9E-7, 0.0, &u);
	zassert_true(rc == -EINVAL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(u != u);
}

ZTEST(zsl_tests, test_phy_elec_potential)
{
	int rc;
	zsl_real_t v;

	rc = zsl_phy_elec_potential(2.1E-9, 0.5, &v);
	zassert_true(rc == 0);
#ifdef CONFIG_ZSL_SINGLE_PRECISION
 	zassert_true(val_is_equal(v, 37.7477175054, 1E-5));
#else
	zassert_true(val_is_equal(v, 37.7477175054, 1E-8));
#endif

	/* Example for negative distance. */
	rc = zsl_phy_elec_potential(2.1E-9, -0.5, &v);
	zassert_true(rc == -EINVAL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(v != v);

	/* Example for zero distance. */
	rc = zsl_phy_elec_potential(2.1E-9, 0.0, &v);
	zassert_true(rc == -EINVAL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(v != v);
}

ZTEST(zsl_tests, test_phy_elec_flux)
{
	int rc;
	zsl_real_t fl;

	rc = zsl_phy_elec_flux(32.3, 1.1, 0.9, &fl);
	zassert_true(rc == 0);
	zassert_true(val_is_equal(fl, 22.0858021727, 1E-6));

	/* Example for negative electric field. */
	rc = zsl_phy_elec_flux(-32.3, 1.1, 0.9, &fl);
	zassert_true(rc == -EINVAL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(fl != fl);

	/* Example for negative area. */
	rc = zsl_phy_elec_flux(32.3, -1.1, 0.9, &fl);
	zassert_true(rc == -EINVAL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(fl != fl);
}
