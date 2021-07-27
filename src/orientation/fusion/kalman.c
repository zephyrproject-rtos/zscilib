/*
 * Copyright (c) 2021 Marti Riba Pons
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <zsl/orientation/fusion/fusion.h>
#include <zsl/orientation/fusion/kalman.h>

static uint32_t zsl_fus_kalm_freq = 0;


static int zsl_fus_kalman(struct zsl_vec *g, struct zsl_vec *a,
			  struct zsl_vec *m, zsl_real_t *var_g, struct zsl_quat *q)
{
	int rc = 0;

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure that the input vectors are tridimensional. */
	if (g->sz != 3 || a->sz != 3 || (m != NULL && (m->sz != 3)) ) {
		rc = -EINVAL;
		goto err;
	}
	/* Make sure that the input quaternion is not zero. */
	if (ZSL_ABS(zsl_quat_magn(q)) < 1E-6) {
		rc = -EINVAL;
		goto err;
	}
#endif

	/* Calculate an estimation of the orientation using only the data of the
	 * gyroscope and quaternion integration. */
	zsl_quat_from_ang_vel(g, q, 1.0 / zsl_fus_kalm_freq, q);
	
	// ZSL_MATRIX_DEF(f, 4, 4);
	// ZSL_MATRIX_DEF(ft, 4, 4);

	// zsl_real_t if2 = 1.0 / (2.0 * zsl_fus_kalm_freq);

	// zsl_real_t f_data [16] = {
	// 	1.0,    -if2 * g->data[0],    -if2 * g->data[1],    -if2 * g->data[2],
	// 	if2 * g->data[0],    1.0,      if2 * g->data[2],    -if2 * g->data[1],
	// 	if2 * g->data[1],    -if2 * g->data[2],    1.0,      if2 * g->data[0],
	// 	if2 * g->data[2],     if2 * g->data[1],    -if2 * g->data[0],     1.0,
	// };

	// zsl_mtx_from_arr(&f, f_data);

	// zsl_mtx_trans(&f, &ft);
	

err:
	return rc;
}

int zsl_fus_kalm_init(uint32_t freq, void* cfg)
{
	int rc = 0;

	struct zsl_fus_kalm_cfg *mcfg = cfg;

	(void)mcfg;

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure that the sample frequency is positive. */
	if (freq <= 0.0) {
		rc = -EINVAL;
		goto err;
	}
#endif

	zsl_fus_kalm_freq = freq;

err:
	return rc;
}

int zsl_fus_kalm_feed(struct zsl_vec *a, struct zsl_vec *m, struct zsl_vec *g,
		      struct zsl_quat *q, void* cfg)
{
    struct zsl_fus_kalm_cfg *mcfg = cfg;

	return zsl_fus_kalman(g, a, m, &(mcfg->var_g), q);
}

void zsl_fus_kalm_error(int error)
{
	/* ToDo: Log error in default handler. */
}
