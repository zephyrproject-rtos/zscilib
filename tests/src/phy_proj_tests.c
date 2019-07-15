/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ztest.h>
#include <zsl/zsl.h>
#include <zsl/matrices.h>
#include <zsl/vectors.h>
#include <zsl/physics/projectiles.h>
#include "floatcheck.h"

void test_phy_proj_init_vel(void)
{
        int rc;
        zsl_real_t vi_hor;
        zsl_real_t vi_ver;

        rc = zsl_phy_proj_init_vel(10.0, ZSL_PI / 6, &vi_hor, &vi_ver);
        zassert_true(rc == 0, NULL);
        zassert_true(val_is_equal(vi_hor, 8.660254038, 1E-6), NULL);
        zassert_true(val_is_equal(vi_ver, 5.0, 1E-6), NULL);
}

void test_phy_proj_time(void)
{
        int rc;
        zsl_real_t t;

        rc = zsl_phy_proj_time(8.0, 20.0, 10.0, &t);
        zassert_true(rc == 0, NULL);
        zassert_true(val_is_equal(t, 2.460370769, 1E-6), NULL);

        rc = zsl_phy_proj_time(8.0, 20.0, 30.0, &t);
        zassert_true(rc == -EINVAL, NULL);
        /* IEEE standard states that x != x is true only for NAN values. */
        zassert_true(t != t, NULL);
}

void test_phy_proj_time2(void)
{
        int rc;
        zsl_real_t t;

        rc = zsl_phy_proj_time2(20.0, -30.0, &t);
        zassert_true(rc == 0, NULL);
        zassert_true(val_is_equal(t, 5.098399103, 1E-6), NULL);

        rc = zsl_phy_proj_time2(20.0, 30.0, &t);
        zassert_true(rc == -EINVAL, NULL);
        /* IEEE standard states that x != x is true only for NAN values. */
        zassert_true(t != t, NULL);
}
