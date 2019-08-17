/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <math.h>
#include <errno.h>
#include <zsl/zsl.h>
#include <zsl/physics/momentum.h>

int
zsl_phy_mom_mom(zsl_real_t m, zsl_real_t v, zsl_real_t *p)
{
	if (m < 0) {
		*p = NAN;
		return -EINVAL;
	}

	*p = m * v;

	return 0;
}

int
zsl_phy_mom_imp(zsl_real_t f, zsl_real_t time, zsl_real_t *imp)
{
	if (time < 0) {
		*imp = NAN;
		return -EINVAL;
	}

	*imp = f * time;

	return 0;
}

int
zsl_phy_mom_change(zsl_real_t m, zsl_real_t vi, zsl_real_t vf, zsl_real_t *p)
{
	if (m < 0) {
		*p = NAN;
		return -EINVAL;
	}

	*p = m * (vf - vi);

	return 0;
}

int
zsl_phy_mom_elas(zsl_real_t m1, zsl_real_t vi1, zsl_real_t vf1, zsl_real_t m2,
		 zsl_real_t vi2, zsl_real_t *vf2)
{
	if (m1 < 0 || m2 <= 0) {
		*vf2 = NAN;
		return -EINVAL;
	}
	
	/*
	 * m1 * vi1 + m2 * vi2 = m1 * vf1 + m2 * vf2
	 */	
	
	*vf2 = (m1 * (vi1 - vf1) + m2 * vi2) / m2;

	return 0;
}

int
zsl_phy_mom_inelas(zsl_real_t m1, zsl_real_t vi1, zsl_real_t m2, zsl_real_t vi2,
		   zsl_real_t *vf)
{
	if (m1 < 0 || m2 < 0 || (m1 + m2) == 0) {
		*vf = NAN;
		return -EINVAL;
	}
	
	/*
	 * m1 * vi1 + m2 * vi2 = (m1 + m2) * vf
	 */	
	
	*vf = (m1 * vi1 + m2 * vi2) / (m1 + m2);

	return 0;
}
