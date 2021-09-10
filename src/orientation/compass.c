/*
 * Copyright (c) 2021 Marti Riba Pons
 * Copyright (c) 2021 Kevin Townsend
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <zsl/orientation/compass.h>

int zsl_comp_dms_to_dd(zsl_real_t d, zsl_real_t m, zsl_real_t s, zsl_real_t *dd)
{
	int rc = 0;

	*dd = 0.0;

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Minutes should always be positive and less than 60. */
	if ((m < 0.0) || (m > 60.0)) {
		rc = -EINVAL;
		goto err;
	}

	/* Seconds should always be positive and less than 60. */
	if ((s < 0.0) || (s > 60.0)) {
		rc = -EINVAL;
		goto err;
	}

	/* Degrees longitude must be between -180.0 and 180.0, and latitude must be
	 * between -90.0 and 90.0. Since we don't know which is being converted,
	 * the best we can do is limit this to the worst-case scenario.
	 */
	if ((d < -180.0) || (d > 180.0)) {
		rc = -EINVAL;
		goto err;
	}
#endif

	/* Adjust for negative values with south and west. */
	if (d < 0.0) {
		/* South and west. */
		*dd = d - (m / 60.0) - (s / 3600.0);
	} else {
		/* North and east. */
		*dd = d + (m / 60.0) + (s / 3600.0);
	}

err:
	return rc;
}

int zsl_comp_magn_north(struct zsl_vec *m, zsl_real_t *d)
{
	int rc = 0;

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure that the input vector have dimension 3. */
	if (m->sz != 3) {
		rc = -EINVAL;
		goto err;
	}
#endif

	if (ZSL_ABS(m->data[1]) < 1E-6) {
		if (m->data[0] > 0.0) {
			*d = 0.0;
		} else {
			*d = 180.0;
		}
	} else if (m->data[1] > 0.0) {
		*d = 90.0 - ZSL_ATAN2(m->data[0], m->data[1]) * 180.0 / ZSL_PI;
	} else if (m->data[1] < 0.0) {
		*d = 270.0 - ZSL_ATAN2(m->data[0], m->data[1]) * 180.0 / ZSL_PI;
	}

err:
	return rc;
}

int zsl_comp_geo_north(struct zsl_vec *m, zsl_real_t dec, zsl_real_t *d)
{
	int rc = 0;

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure that the input vector have dimension 3. */
	if (m->sz != 3) {
		rc = -EINVAL;
		goto err;
	}
#endif

	zsl_comp_magn_north(m, d);
	*d += dec;

err:
	return rc;
}
