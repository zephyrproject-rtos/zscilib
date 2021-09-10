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

static uint32_t zsl_fus_kalm_freq;
static uint32_t zsl_fus_kalm_initialised;

static int zsl_fus_kalman(struct zsl_vec *g, struct zsl_vec *a,
			  struct zsl_vec *m, zsl_real_t *var_g, zsl_real_t *var_a,
			  zsl_real_t *var_m, zsl_real_t *incl, struct zsl_mtx *P,
			  struct zsl_quat *q)
{
	int rc = 0;

#if CONFIG_ZSL_BOUNDS_CHECKS
	if (a == NULL || a->sz != 3 || m == NULL || m->sz != 3 || g == NULL ||
	    g->sz != 3) {
		rc = -EINVAL;
		goto err;
	}
#endif

	/* PREDICTION STEP. */

	/* Useful constant to reduce the code. */
	zsl_real_t if2 = 1.0 / (2.0 * zsl_fus_kalm_freq);

	/* Calculate the matrix F and its transpose. */
	ZSL_MATRIX_DEF(F, 4, 4);
	ZSL_MATRIX_DEF(Ft, 4, 4);

	zsl_real_t F_data[16] = {
		1.0, -if2 * g->data[0], -if2 * g->data[1], -if2 * g->data[2],
		if2 * g->data[0], 1.0, if2 * g->data[2], -if2 * g->data[1],
		if2 * g->data[1], -if2 * g->data[2], 1.0, if2 * g->data[0],
		if2 * g->data[2], if2 * g->data[1], -if2 * g->data[0], 1.0,
	};

	zsl_mtx_from_arr(&F, F_data);
	zsl_mtx_trans(&F, &Ft);

	/* Calculate the matrix W and its transpose. */
	ZSL_MATRIX_DEF(W, 4, 3);
	ZSL_MATRIX_DEF(Wt, 3, 4);

	zsl_real_t W_data[12] = {
		-q->i, -q->j, -q->k,
		q->r, -q->k,  q->j,
		q->k,  q->r, -q->i,
		-q->j,  q->i,  q->r
	};

	zsl_mtx_from_arr(&W, W_data);
	zsl_mtx_trans(&W, &Wt);

	/* Calculate the matrix Q. */
	ZSL_MATRIX_DEF(Q, 4, 4);
	zsl_mtx_mult(&W, &Wt, &Q);
	zsl_mtx_scalar_mult_d(&Q, *var_g * if2 * if2);

	/* Calculate the matrix P. */
	ZSL_MATRIX_DEF(FP, 4, 4);
	zsl_mtx_mult(&F, P, &FP);
	zsl_mtx_mult(&FP, &Ft, P);
	zsl_mtx_add_d(P, &Q);

	/* Calculate an estimation of the orientation using only the data of the
	 * gyroscope and quaternion integration. */
	zsl_quat_from_ang_vel(g, q, 1.0 / zsl_fus_kalm_freq, q);

	/* CORRECTION STEP. */

	/* Normalize the magnetometer and accelerometer data. */
	zsl_vec_to_unit(a);
	zsl_vec_to_unit(m);

	/* Define the measurement vector 'z'. */
	ZSL_MATRIX_DEF(z, 6, 1);
	z.data[0] = a->data[0];
	z.data[1] = a->data[1];
	z.data[2] = a->data[2];
	z.data[3] = m->data[0];
	z.data[4] = m->data[1];
	z.data[5] = m->data[2];

	/* Turn the data of the magnetometer into a pure quaterion. */
	struct zsl_quat qm = {
		.r = 0.0,
		.i = m->data[0],
		.j = m->data[1],
		.k = m->data[2]
	};

	/* Define the normalized gravity and magnetic field in the global NED
	 * frame. */
	struct zsl_quat gv = { .r = 0.0, .i = 0.0, .j = 0.0, .k = -1.0 };
	struct zsl_quat mg = { .r = 0.0, .i = 0.0, .j = 0.0, .k = 0.0 };
	if (incl == NULL) {
		struct zsl_quat h;
		zsl_quat_rot(q, &qm, &h);
		mg.i = ZSL_SQRT(h.i * h.i + h.j * h.j);
		mg.k = h.k;
	} else {
		mg.i = ZSL_COS(*incl * ZSL_PI / 180.0);
		mg.k = ZSL_SIN(*incl * ZSL_PI / 180.0);
	}

	zsl_quat_to_unit_d(&mg);

	/* Calculate the measurement model, h. To do this, use the predicted
	 * orientation 'q' to rotate the quaternions 'grav' and 'magn'. */
	ZSL_MATRIX_DEF(h, 6, 1);
	struct zsl_quat a2;
	struct zsl_quat m2;

	zsl_quat_rot(q, &gv, &a2);
	zsl_quat_rot(q, &mg, &m2);

	h.data[0] = 2.0 * a2.i;
	h.data[1] = 2.0 * a2.j;
	h.data[2] = 2.0 * a2.k;
	h.data[3] = 2.0 * m2.i;
	h.data[4] = 2.0 * m2.j;
	h.data[5] = 2.0 * m2.k;

	/* Calculate H, which is the Jacobian matrix of h. Calculate its
	 * transpose too. */
	ZSL_MATRIX_DEF(H, 6, 4);
	ZSL_MATRIX_DEF(Ht, 4, 6);

	zsl_real_t H_data[24] = {
		/* Fisrt 3 rows of the H matrix. */
		q->j, -q->k, q->r, -q->i,
		-q->i, -q->r, -q->k, -q->j,
		0.0, 2.0 * q->i, 2.0 * q->j, 0.0,

		/* Fourth row of the H matrix. */
		-mg.k * q->j, mg.k * q->k, -2.0 * mg.i * q->j - mg.k * q->r,
		-2.0 * mg.i * q->k + mg.k * q->i,

		/* Fifth row of the H matrix. */
		-mg.i * q->k + mg.k * q->i,  mg.i * q->j + mg.k * q->r,
		mg.i * q->i + mg.k * q->k, -mg.i * q->r + mg.k * q->j,

		/* Sixth row of the H matrix. */
		mg.i * q->j, mg.i * q->k - 2.0 * mg.k * q->i,
		mg.i * q->r - 2.0 * mg.k * q->j, mg.i * q->i
	};

	zsl_mtx_from_arr(&H, H_data);
	zsl_mtx_scalar_mult_d(&H, 2.0);
	zsl_mtx_trans(&H, &Ht);

	/* Define the R matrix with the variance of the accelerometer
	 * and magnetometer. */
	ZSL_MATRIX_DEF(R, 6, 6);
	zsl_mtx_init(&R, zsl_mtx_entry_fn_identity);
	zsl_mtx_set(&R, 0, 0, *var_a);
	zsl_mtx_set(&R, 1, 1, *var_a);
	zsl_mtx_set(&R, 2, 2, *var_a);
	zsl_mtx_set(&R, 3, 3, *var_m);
	zsl_mtx_set(&R, 4, 4, *var_m);
	zsl_mtx_set(&R, 5, 5, *var_m);

	/* Define the vector v, the matrix S and the matrix K. */
	ZSL_MATRIX_DEF(v, 6, 1);
	ZSL_MATRIX_DEF(S, 6, 6);
	ZSL_MATRIX_DEF(K, 4, 6);

	/* Calculation of v. */
	zsl_mtx_sub(&z, &h, &v);

	/* Calculation of S. */
	ZSL_MATRIX_DEF(HP, 6, 4);
	ZSL_MATRIX_DEF(HPHt, 6, 6);
	zsl_mtx_mult(&H, P, &HP);
	zsl_mtx_mult(&HP, &Ht, &HPHt);
	zsl_mtx_add(&HPHt, &R, &S);

	/* Calculation of K. */
	ZSL_MATRIX_DEF(S_inv, 6, 6);
	ZSL_MATRIX_DEF(PHt, 4, 6);
	zsl_mtx_inv(&S, &S_inv);
	zsl_mtx_mult(P, &Ht, &PHt);
	zsl_mtx_mult(&PHt, &S_inv, &K);

	/* Calculate the corrected matrix P. */
	ZSL_MATRIX_DEF(idx, 4, 4);
	ZSL_MATRIX_DEF(KH, 4, 4);
	zsl_mtx_init(&idx, zsl_mtx_entry_fn_identity);
	zsl_mtx_mult(&K, &H, &KH);
	zsl_mtx_sub_d(&idx, &KH);
	zsl_mtx_mult(&idx, P, P);

	/* Calculate the corrected orientation quaternion q. */
	ZSL_MATRIX_DEF(Kv, 4, 1);
	zsl_mtx_mult(&K, &v, &Kv);

	q->r += Kv.data[0];
	q->i += Kv.data[1];
	q->j += Kv.data[2];
	q->k += Kv.data[3];

	/* Normalize the output quaternion. */
	zsl_quat_to_unit_d(q);

err:
	return rc;
}

static int zsl_fus_kalm_quat_init(struct zsl_vec *a, struct zsl_vec *m,
				  struct zsl_quat *q)
{
	int rc = 0;

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure that the input vectors are tridimensional. */
	if (a == NULL || a->sz != 3 || m == NULL || m->sz != 3) {
		rc = -EINVAL;
		goto err;
	}
#endif

	/* Define the rotation matrix and two helper vectors. */
	ZSL_MATRIX_DEF(mtx, 3, 3);
	ZSL_VECTOR_DEF(v, 3);
	ZSL_VECTOR_DEF(w, 3);
	ZSL_VECTOR_DEF(u, 3);

	/* Set the first column of the rotation matrix as (a x m) x a (cross
	 * product), the second column as (a x m) and the third one as 'a'. */
	zsl_vec_cross(a, m, &w);
	zsl_vec_cross(&w, a, &v);
	zsl_vec_copy(&u, a);

	zsl_vec_to_unit(&v);
	zsl_vec_to_unit(&w);
	zsl_vec_to_unit(&u);

	zsl_mtx_set_col(&mtx, 0, v.data);
	zsl_mtx_set_col(&mtx, 1, w.data);
	zsl_mtx_set_col(&mtx, 2, u.data);

	/* The initial quaternion is obtained converting this rotation matrix into
	 * a unit quaternion. */
	zsl_quat_from_rot_mtx(&mtx, q);
	zsl_quat_to_unit_d(q);

	/* Inidicate that we have already called this function. */
	zsl_fus_kalm_initialised = 1;

err:
	return rc;
}

static int zsl_fus_kalm_P_init(struct zsl_mtx *P)
{
	int rc = 0;

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure that the input matrix P has dimension 4x4. */
	if (P->sz_rows != 4 || P->sz_cols != 4) {
		rc = -EINVAL;
		goto err;
	}
#endif

	zsl_mtx_init(P, zsl_mtx_entry_fn_identity);

	/* Inidicate that we have already called this function. */
	zsl_fus_kalm_initialised = 1;

err:
	return rc;
}

int zsl_fus_kalm_init(uint32_t freq, void *cfg)
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
		      zsl_real_t *incl, struct zsl_quat *q, void *cfg)
{
	struct zsl_fus_kalm_cfg *mcfg = cfg;

	if (mcfg->var_g < 0.0 || mcfg->var_a < 0.0 || mcfg->var_m < 0.0) {
		return -EINVAL;
	}

	/* This functions should only be called once. */
	if (!zsl_fus_kalm_initialised) {
		zsl_fus_kalm_quat_init(a, m, q);
		zsl_fus_kalm_P_init(&(mcfg->P));
	}

	return zsl_fus_kalman(g, a, m, &(mcfg->var_g), &(mcfg->var_a),
			      &(mcfg->var_m), incl, &(mcfg->P), q);
}

void zsl_fus_kalm_error(int error)
{
	/* ToDo: Log error in default handler. */
}
