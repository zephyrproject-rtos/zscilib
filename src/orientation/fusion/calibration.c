/*
 * Copyright (c) 2021 Marti Riba Pons
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <zsl/orientation/fusion/fusion.h>
#include <zsl/orientation/fusion/calibration.h>
#include <zsl/statistics.h>

#ifndef CONFIG_ZSL_SINGLE_PRECISION
static zsl_real_t zsl_fus_cal_magn_f_shp(struct zsl_vec *H, struct zsl_vec *b)
{
	ZSL_VECTOR_DEF(Hb, 3);
	zsl_vec_add(H, b, &Hb);

	return zsl_vec_norm(&Hb);
}
#endif

#ifndef CONFIG_ZSL_SINGLE_PRECISION
static zsl_real_t zsl_fus_cal_magn_f_elli(struct zsl_vec *H, struct zsl_mtx *g)
{
	ZSL_MATRIX_DEF(K, 3, 3);
	K.data[0] = g->data[0];
	K.data[1] = K.data[3] = g->data[1];
	K.data[2] = K.data[6] = g->data[2];
	K.data[4] = g->data[3];
	K.data[5] = K.data[7] = g->data[4];
	K.data[8] = g->data[5];

	ZSL_MATRIX_DEF(Hb, 3, 1);
	Hb.data[0] = H->data[0] + g->data[6];
	Hb.data[1] = H->data[1] + g->data[7];
	Hb.data[2] = H->data[2] + g->data[8];

	ZSL_MATRIX_DEF(KHb, 3, 1);
	zsl_mtx_mult(&K, &Hb, &KHb);

	ZSL_VECTOR_DEF(Hd, 3);
	zsl_vec_from_arr(&Hd, KHb.data);

	return zsl_vec_norm(&Hd);
}
#endif

int zsl_fus_cal_rot_mtx(struct zsl_vec *v, struct zsl_mtx *m,
			struct zsl_vec *v_rot)
{
	int rc = 0;

#if CONFIG_ZSL_BOUNDS_CHECKS
	if (v->sz != 3 || v_rot->sz != 3 || m->sz_rows != 3 || m->sz_cols != 3) {
		rc = -EINVAL;
		goto err;
	}
#endif

	ZSL_MATRIX_DEF(vm, 3, 1);
	zsl_mtx_from_arr(&vm, v->data);

	ZSL_MATRIX_DEF(mv, 3, 1);
	zsl_mtx_mult(m, &vm, &mv);

	zsl_vec_from_arr(v_rot, mv.data);

err:
	return rc;
}

int zsl_fus_cal_rot_axis_angle(struct zsl_vec *v, struct zsl_vec *a,
			       zsl_real_t *b, struct zsl_vec *v_rot)
{
	int rc = 0;

#if CONFIG_ZSL_BOUNDS_CHECKS
	if (v->sz != 3 || v_rot->sz != 3 || a->sz != 3) {
		rc = -EINVAL;
		goto err;
	}
#endif

	struct zsl_quat q;
	ZSL_MATRIX_DEF(m, 3, 3);
	zsl_quat_from_axis_angle(a, b, &q);
	zsl_quat_to_rot_mtx(&q, &m);

	zsl_fus_cal_rot_mtx(v, &m, v_rot);

err:
	return rc;
}

#ifndef CONFIG_ZSL_SINGLE_PRECISION
int zsl_fus_cal_magn(struct zsl_mtx *m, zsl_real_t *l, zsl_real_t *mu,
		     struct zsl_mtx *K, struct zsl_vec *b)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure that the input magnetometer data matrix has dimension N x 3.
	 * 'K' must be a 3 x 3 matrix and 'b' a tridimensional vector. The
	 * 'mu' parameter must be greater than one. */
	if (m->sz_cols != 3 || K->sz_rows != 3 || K->sz_cols != 3 ||
	    b->sz != 3 || *mu < 1.0) {
		return -EINVAL;
	}
#endif

	/* Make a copy of l. */
	zsl_real_t l_copy = *l;


	/* SPHERE FITTING. */

	/* Calculate the initial value of beta (b) as the mean value of (x, y, z)
	 * max and (x, y, z) min. */
	ZSL_VECTOR_DEF(v, m->sz_rows);
	ZSL_VECTOR_DEF(w, m->sz_rows);

	zsl_real_t mean = 0.0;

	for (size_t i = 0; i < 3; i++) {
		zsl_mtx_get_col(m, i, v.data);
		zsl_vec_sort(&v, &w);
		b->data[i] = -(w.data[m->sz_rows - 1] + w.data[0]) / 2.0;
		mean += (w.data[m->sz_rows - 1] - w.data[0]) / 2.0;
	}

	/* R is a first estimation of the radius for the sphere fitting. */
	zsl_real_t R = mean / 3.0;

	/* Define needed variables. */
	ZSL_VECTOR_DEF(H, 3);
	ZSL_MATRIX_DEF(J, 1, 4);
	ZSL_MATRIX_DEF(Jt, 4, 1);
	ZSL_MATRIX_DEF(JtJ, 4, 4);
	ZSL_MATRIX_DEF(idx, 4, 4);
	ZSL_MATRIX_DEF(t, 4, 1);
	zsl_real_t f, S = 0.0, S2;

	/* Initialize the identity matrix. */
	zsl_mtx_init(&idx, zsl_mtx_entry_fn_identity);

	for (size_t j = 0; j < m->sz_rows; j++) {

		/* Calculate the Jacobian matrix J. */
		zsl_mtx_init(&J, NULL);
		for (size_t i = 0; i < m->sz_rows; i++) {
			zsl_mtx_get_row(m, i, H.data);
			f = zsl_fus_cal_magn_f_shp(&H, b);
			J.data[0] += 1.0;
			J.data[1] += -(H.data[0] - b->data[0]) / f;
			J.data[2] += -(H.data[1] - b->data[1]) / f;
			J.data[3] += -(H.data[2] - b->data[2]) / f;
		}

		/* Compute the transpose of J. */
		zsl_mtx_trans(&J, &Jt);

		/* Calculate the value of tau ('t'). */
		zsl_mtx_get_row(m, j, H.data);
		f = zsl_fus_cal_magn_f_shp(&H, b);
		zsl_mtx_mult(&Jt, &J, &JtJ);
		idx.data[0] = *l * JtJ.data[0];
		idx.data[5] = *l * JtJ.data[5];
		idx.data[10] = *l * JtJ.data[10];
		idx.data[15] = *l * JtJ.data[15];
		zsl_mtx_add_d(&JtJ, &idx);
		zsl_mtx_inv(&JtJ, &idx);
		zsl_mtx_mult(&idx, &Jt, &t);
		zsl_mtx_scalar_mult_d(&t, -(R - f));

		/* Calculate the new pair (R, b) by adding tau to the old values of R
		 * and b. */
		R += t.data[0];
		b->data[0] += t.data[1];
		b->data[1] += t.data[2];
		b->data[2] += t.data[3];

		if (j < (m->sz_rows - 1)) {

			/* Calculate the squared residual sum of all samples. */
			zsl_mtx_get_row(m, j + 1, H.data);
			f = zsl_fus_cal_magn_f_shp(&H, b);
			S2 = (S * j + (R - f) * (R - f)) / (zsl_real_t) (j + 1);

			/* Update the value of lambda (l) with the information from the
			 * squared residual sum. */
			if (S2 < S) {
				*l /= *mu;
			} else {
				*l *= *mu;
			}

			S = S2;
		}
	}

	/* ELLIPSOID FITTING. */

	/* Initialize the matrix K as an identity matrix. */
	zsl_mtx_init(K, zsl_mtx_entry_fn_identity);

	/* Reset the value of lambda ('l') and the value of the sum (S). */
	*l = l_copy;
	S = 0.0;

	/* Define gamma (g) and initialize it with the calculated values of
	 * beta in the sphere fitting. */
	ZSL_MATRIX_DEF(g, 9, 1);
	g.data[0] = 1.0;
	g.data[1] = 0.0;
	g.data[2] = 0.0;
	g.data[3] = 1.0;
	g.data[4] = 0.0;
	g.data[5] = 1.0;
	g.data[6] = b->data[0];
	g.data[7] = b->data[1];
	g.data[8] = b->data[2];

	/* Define needed variables. */
	ZSL_MATRIX_DEF(N, 1, 9);
	ZSL_MATRIX_DEF(Nt, 9, 1);
	ZSL_MATRIX_DEF(NtN, 9, 9);
	ZSL_MATRIX_DEF(idxN, 9, 9);
	ZSL_MATRIX_DEF(tN, 9, 1);
	zsl_real_t A, B, C;

	/* Initialize the identity matrix. */
	zsl_mtx_init(&idxN, zsl_mtx_entry_fn_identity);

	/* Start the ellipsoid fitting algorithm. */
	for (size_t j = 0; j < m->sz_rows; j++) {
		zsl_mtx_init(&N, NULL);

		for (size_t i = 0; i < m->sz_rows; i++) {
			zsl_mtx_get_row(m, i, H.data);
			f = zsl_fus_cal_magn_f_elli(&H, &g);

			/* Define variables for repeated calculations. */
			A = g.data[0] * (H.data[0] + b->data[0]) +
			    g.data[1] * (H.data[1] + b->data[1]) +
			    g.data[2] * (H.data[2] + b->data[2]);
			B = g.data[1] * (H.data[0] + b->data[0]) +
			    g.data[3] * (H.data[1] + b->data[1]) +
			    g.data[4] * (H.data[2] + b->data[2]);
			C = g.data[2] * (H.data[0] + b->data[0]) +
			    g.data[4] * (H.data[1] + b->data[1]) +
			    g.data[5] * (H.data[2] + b->data[2]);

			/* Calculate the Jacobian matrix N. */
			N.data[0] += -(H.data[0] + b->data[0]) * A / f;
			N.data[1] += -(H.data[1] + b->data[1]) * B / f;
			N.data[2] += -(H.data[2] + b->data[2]) * C / f;
			N.data[3] += -((H.data[1] + b->data[1]) * A +
				       (H.data[0] + b->data[0]) * B) / f;
			N.data[4] += -((H.data[2] + b->data[2]) * A +
				       (H.data[0] + b->data[0]) * C) / f;
			N.data[5] += -((H.data[2] + b->data[2]) * B +
				       (H.data[1] + b->data[1]) * C) / f;
			N.data[6] += -A / f;
			N.data[7] += -B / f;
			N.data[8] += -C / f;
		}

		/* Calculate the transpose of N. */
		zsl_mtx_trans(&N, &Nt);

		/* Calculate the value of tau (tN). */
		zsl_mtx_get_row(m, j, H.data);
		f = zsl_fus_cal_magn_f_elli(&H, &g);
		zsl_mtx_mult(&Nt, &N, &NtN);
		idxN.data[0] = *l * NtN.data[0];
		idxN.data[10] = *l * NtN.data[10];
		idxN.data[20] = *l * NtN.data[20];
		idxN.data[30] = *l * NtN.data[30];
		idxN.data[40] = *l * NtN.data[40];
		idxN.data[50] = *l * NtN.data[50];
		idxN.data[60] = *l * NtN.data[60];
		idxN.data[70] = *l * NtN.data[70];
		idxN.data[80] = *l * NtN.data[80];
		zsl_mtx_add_d(&NtN, &idxN);
		zsl_mtx_inv(&NtN, &idxN);
		zsl_mtx_mult(&idxN, &Nt, &tN);
		zsl_mtx_scalar_mult_d(&tN, -(R - f));

		/* Calculate the new vector gamma. */
		g.data[0] += tN.data[0];
		g.data[1] += tN.data[1];
		g.data[2] += tN.data[2];
		g.data[3] += tN.data[3];
		g.data[4] += tN.data[4];
		g.data[5] += tN.data[5];
		g.data[6] += tN.data[6];
		g.data[7] += tN.data[7];
		g.data[8] += tN.data[8];

		if (j < (m->sz_rows - 1)) {

			/* Calculate the squared residual sum of all samples. */
			zsl_mtx_get_row(m, j + 1, H.data);
			f = zsl_fus_cal_magn_f_elli(&H, &g);
			S2 = (S * j + (R - f) * (R - f)) / (zsl_real_t) (j + 1);

			/* Update the value of lambda (l) with the information from the
			 * squared residual sum. */
			if (S2 < S) {
				*l /= *mu;
			} else {
				*l *= *mu;
			}

			S = S2;
		}
	}

	/* Calculate the matrix K. */
	K->data[0] = g.data[0];
	K->data[1] = K->data[3] = g.data[1];
	K->data[2] = K->data[6] = g.data[2];
	K->data[4] = g.data[3];
	K->data[5] = K->data[7] = g.data[4];
	K->data[8] = g.data[5];

	/* Calculate the vector beta ('b'). */
	b->data[0] = g.data[6];
	b->data[1] = g.data[7];
	b->data[2] = g.data[8];

	return 0;
}
#endif

#ifndef CONFIG_ZSL_SINGLE_PRECISION
int zsl_fus_cal_magn_fast(struct zsl_mtx *m, zsl_real_t *me, struct zsl_mtx *K,
			  struct zsl_vec *b)
{

	zsl_real_t approx_me = 50.0;

	/* Use an approximation for me if no value provided. */
	if (me == NULL) {
		/* Locations vary fromo 25-65 uT, but most are close to 50 uT. */
		me = &approx_me;
	}

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure that the input magnetometer data matrix has dimension N x 3.
	 * 'K' must be a 3 x 3 matrix and 'b' a tridimensional vector. Also 'me'
	 * should be positive. */
	if (m->sz_cols != 3 || K->sz_rows != 3 || K->sz_cols != 3 ||
	    b->sz != 3 || *me < 0.0) {
		return -EINVAL;
	}
#endif

	ZSL_VECTOR_DEF(coef, 9);
	zsl_sta_quad_fit(m, &coef);

	ZSL_MATRIX_DEF(A, 3, 3);
	ZSL_MATRIX_DEF(v, 3, 1);
	A.data[0] = coef.data[0];
	A.data[1] = A.data[3] = coef.data[3];
	A.data[2] = A.data[6] = coef.data[4];
	A.data[4] = coef.data[1];
	A.data[5] = A.data[7] = coef.data[5];
	A.data[8] = coef.data[2];

	v.data[0] = coef.data[6];
	v.data[1] = coef.data[7];
	v.data[2] = coef.data[8];

	ZSL_MATRIX_DEF(X0, 3, 1);
	ZSL_MATRIX_DEF(L, 3, 3);
	ZSL_MATRIX_DEF(G, 3, 3);
	ZSL_MATRIX_DEF(Ai, 3, 3);

	zsl_mtx_cholesky(&A, &L);

	zsl_mtx_trans(&L, &G);
	zsl_mtx_scalar_mult_d(&G, *me);
	zsl_mtx_copy(K, &G);

	zsl_mtx_inv(&A, &Ai);
	zsl_mtx_mult(&Ai, &v, &X0);

	b->data[0] = X0.data[0];
	b->data[1] = X0.data[1];
	b->data[2] = X0.data[2];

	return 0;
}
#endif

int zsl_fus_cal_corr_scalar(zsl_real_t *d, zsl_real_t *k, zsl_real_t *b,
			    zsl_real_t *d_out)
{
	int rc = 0;

	*d_out = *k * (*d + *b);

	return rc;
}

int zsl_fus_cal_corr_vec(struct zsl_vec *v, struct zsl_mtx *K,
			 struct zsl_vec *b, struct zsl_vec *v_out)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure that the input vectors and matrices have dimension the
	 * same dimensions. */
	if (v->sz != v_out->sz || v->sz != K->sz_rows || v->sz != K->sz_cols ||
	    v->sz != b->sz) {
		return -EINVAL;
	}
#endif

	ZSL_VECTOR_DEF(vb, v->sz);
	ZSL_MATRIX_DEF(VB, v->sz, 1);
	ZSL_MATRIX_DEF(KVB, v->sz, 1);

	zsl_vec_add(v, b, &vb);
	zsl_mtx_from_arr(&VB, vb.data);
	zsl_mtx_mult(K, &VB, &KVB);
	zsl_vec_from_arr(v_out, KVB.data);

	return 0;
}

int zsl_fus_cal_madg(struct zsl_mtx *g, struct zsl_mtx *a,
		     struct zsl_mtx *m, zsl_real_t sampleFreq, zsl_real_t *incl,
		     zsl_real_t *beta)
{
	int rc = 0;

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure that the input vectors have dimension 3, and that the
	 * frequency is positive. */
	if (g->sz_cols != 3 || a->sz_cols != 3 || m->sz_cols != 3 ||
	    sampleFreq < 0.0 || g->sz_rows != a->sz_rows ||
	    g->sz_rows != m->sz_rows) {
		rc = -EINVAL;
		goto err;
	}
#endif

	struct zsl_fus_madg_cfg madg_cfg;

	struct zsl_fus_drv madg_drv = {
		.init_handler = zsl_fus_madg_init,
		.feed_handler = zsl_fus_madg_feed,
		.error_handler = zsl_fus_madg_error,
		.config = &madg_cfg,
	};

	struct zsl_quat q;
	struct zsl_euler e = { 0 };
	struct zsl_attitude att;

	ZSL_VECTOR_DEF(gv, 3);
	ZSL_VECTOR_DEF(av, 3);
	ZSL_VECTOR_DEF(mv, 3);

	zsl_real_t Sx, Sy, St, Smin;

	madg_drv.init_handler(sampleFreq, madg_drv.config);

	for (size_t i = 0; i < 1000; i++) {
		zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
		madg_cfg.beta = (zsl_real_t) i / 1000.0;
		Sx = 0.0;
		Sy = 0.0;

		for (size_t j = 0; j < a->sz_rows; j++) {
			zsl_mtx_get_row(g, j, gv.data);
			zsl_mtx_get_row(a, j, av.data);
			zsl_mtx_get_row(m, j, mv.data);

			madg_drv.feed_handler(&av, &mv, &gv, incl, &q, madg_drv.config);

			zsl_quat_to_euler(&q, &e);
			e.x *= 180. / ZSL_PI;
			e.y *= 180. / ZSL_PI;
			e.z *= 180. / ZSL_PI;

			zsl_att_from_accel(&av, &att);

			Sx += (e.x - att.roll) * (e.x - att.roll);
			Sy += (e.y - att.pitch) * (e.y - att.pitch);
		}
		Sx = ZSL_SQRT(Sx / a->sz_rows);
		Sy = ZSL_SQRT(Sy / a->sz_rows);

		St = (Sx + Sy) / 2.0;

		Smin = 0;
		if (i == 0) {
			Smin = St;
			*beta = madg_cfg.beta;
		} else if (St < Smin) {
			Smin = St;
			*beta = madg_cfg.beta;
		}
	}

err:
	return rc;
}

int zsl_fus_cal_mahn(struct zsl_mtx *g, struct zsl_mtx *a,
		     struct zsl_mtx *m, zsl_real_t sampleFreq, zsl_real_t *incl,
		     zsl_real_t *kp)
{
	int rc = 0;

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure that the input vectors have dimension 3, and that the
	 * frequency is positive. */
	if (g->sz_cols != 3 || a->sz_cols != 3 || m->sz_cols != 3 ||
	    sampleFreq < 0.0 || g->sz_rows != a->sz_rows ||
	    g->sz_rows != m->sz_rows) {
		rc = -EINVAL;
		goto err;
	}
#endif

	zsl_real_t _mahn_intfb[3] = { 0.0, 0.0, 0.0 };
	struct zsl_fus_mahn_cfg mahn_cfg = {
		.ki = 0.02,
		.intfb = {
			.sz = 3,
			.data = _mahn_intfb,
		},
	};

	struct zsl_fus_drv mahn_drv = {
		.init_handler = zsl_fus_mahn_init,
		.feed_handler = zsl_fus_mahn_feed,
		.error_handler = zsl_fus_mahn_error,
		.config = &mahn_cfg,
	};

	struct zsl_quat q;
	struct zsl_euler e = { 0 };
	struct zsl_attitude att;

	ZSL_VECTOR_DEF(gv, 3);
	ZSL_VECTOR_DEF(av, 3);
	ZSL_VECTOR_DEF(mv, 3);

	zsl_real_t Sx, Sy, St, Smin;

	mahn_drv.init_handler(sampleFreq, mahn_drv.config);

	for (size_t i = 0; i < 1000; i++) {
		zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
		mahn_cfg.kp = (zsl_real_t) i / 1000.0;
		Sx = 0.0;
		Sy = 0.0;

		for (size_t j = 0; j < a->sz_rows; j++) {
			zsl_mtx_get_row(g, j, gv.data);
			zsl_mtx_get_row(a, j, av.data);
			zsl_mtx_get_row(m, j, mv.data);

			mahn_drv.feed_handler(&av, &mv, &gv, incl, &q, mahn_drv.config);

			zsl_quat_to_euler(&q, &e);
			e.x *= 180. / ZSL_PI;
			e.y *= 180. / ZSL_PI;
			e.z *= 180. / ZSL_PI;

			zsl_att_from_accel(&av, &att);

			Sx += (e.x - att.roll) * (e.x - att.roll);
			Sy += (e.y - att.pitch) * (e.y - att.pitch);
		}
		Sx = ZSL_SQRT(Sx / a->sz_rows);
		Sy = ZSL_SQRT(Sy / a->sz_rows);

		St = (Sx + Sy) / 2.0;

		Smin = 0;
		if (i == 0) {
			Smin = St;
			*kp = mahn_cfg.kp;
		} else if (St < Smin) {
			Smin = St;
			*kp = mahn_cfg.kp;
		}
	}

err:
	return rc;
}
