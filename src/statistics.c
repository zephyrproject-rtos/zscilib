/*
 * Copyright (c) 2021 Marti Riba Pons
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <zsl/zsl.h>
#include <zsl/statistics.h>

int zsl_sta_mean(struct zsl_vec *v, zsl_real_t *m)
{
	zsl_vec_ar_mean(v, m);

	return 0;
}

int zsl_sta_trim_mean(struct zsl_vec *v, zsl_real_t p, zsl_real_t *m)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure p is between 0 and 50. */
	if (p > 50.0 || p < 0.0) {
		return -EINVAL;
	}
#endif

	ZSL_VECTOR_DEF(w, v->sz);
	zsl_real_t per_l, per_h;
	size_t first_val = 0, count = 0;

	*m = 0.0;
	zsl_sta_percentile(v, p, &per_l);
	zsl_sta_percentile(v, 100 - p, &per_h);

	zsl_vec_init(&w);
	zsl_vec_sort(v, &w);

	for (size_t i = 0; i < v->sz; i++) {
		if (w.data[i] >= per_l && w.data[i] <= per_h) {
			count++;
		}
		if (count == 1) {
			first_val = i;
		}
	}

	ZSL_VECTOR_DEF(sub, count);
	zsl_vec_get_subset(&w, first_val, count, &sub);
	zsl_sta_mean(&sub, m);

	return 0;
}

int zsl_sta_weighted_mean(struct zsl_vec *v, struct zsl_vec *w, zsl_real_t *m)
{
	zsl_real_t sumw = 0.0, sumwx = 0.0;

	for (size_t i = 0; i < v->sz; i++) {
		sumw += w->data[i];
	}

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure the vectors dimensions match. */
	if (v->sz != w->sz) {
		return -EINVAL;
	}

	/* Make sure the weights are positive or zero. */
	for (size_t i = 0; i < v->sz; i++) {
		if (w->data[i] < 0.0) {
			return -EINVAL;
		}
	}

	/* Make sure that not all of the weights are zero. */
	if (ZSL_ABS(sumw) < 1E-6) {
		return -EINVAL;
	}
#endif

	for (size_t i = 0; i < v->sz; i++) {
		sumwx += w->data[i] * v->data[i];
	}

	*m = sumwx / sumw;

	return 0;
}

int zsl_sta_time_weighted_mean(struct zsl_vec *v, struct zsl_vec *t,
			       zsl_real_t *m)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure the vectors dimensions match. */
	if (v->sz != t->sz) {
		return -EINVAL;
	}
	/* Make sure that the values in 'v' are positive. */
	for (size_t i = 0; i < v->sz; i++) {
		if (v->data[i] < 0.0) {
			return -EINVAL;
		}
	}
	/* The vector 'x' can't have any repeated values. */
	for (size_t i = 0; i < t->sz; i++) {
		if (zsl_vec_contains(t, t->data[i], 1E-6) > 1) {
			return -EINVAL;
		}
	}
#endif

	ZSL_VECTOR_DEF(ts, t->sz);
	ZSL_VECTOR_DEF(vs, v->sz);

	zsl_real_t sum = 0.0;

	zsl_vec_sort(t, &ts);

	for (size_t i = 0; i < t->sz; i++) {
		for (size_t g = 0; g < t->sz; g++) {
			if (ZSL_ABS(ts.data[i] - t->data[g]) < 1E-6) {
				vs.data[i] = v->data[g];
			}
		}
	}

	for (size_t j = 1; j < t->sz; j++) {
		sum += (ts.data[j] - ts.data[j - 1]) *
		       (vs.data[j] + vs.data[j - 1]) / 2.0;
	}

	*m = sum / (ts.data[(t->sz - 1)] - ts.data[0]);

	return 0;
}

int zsl_sta_demean(struct zsl_vec *v, struct zsl_vec *w)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure the vectors dimensions match. */
	if (v->sz != w->sz) {
		return -EINVAL;
	}
#endif

	zsl_real_t m;

	zsl_vec_copy(w, v);
	zsl_sta_mean(v, &m);
	zsl_vec_scalar_add(w, -m);

	return 0;
}

int zsl_sta_percentile(struct zsl_vec *v, zsl_real_t p, zsl_real_t *val)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure p is between 0 and 100. */
	if (p > 100.0 || p < 0.0) {
		return -EINVAL;
	}
#endif

	ZSL_VECTOR_DEF(w, v->sz);

	zsl_vec_init(&w);
	zsl_vec_sort(v, &w);

	zsl_real_t x = (p * v->sz) / 100.;
	zsl_real_t per = ZSL_FLOOR(x);

	if (x == per) {
		*val = (w.data[(int)per] + w.data[(int)per - 1]) / 2.;
	} else {
		*val = w.data[(int)per];
	}

	return 0;
}

int zsl_sta_median(struct zsl_vec *v, zsl_real_t *m)
{
	zsl_sta_percentile(v, 50, m);

	return 0;
}

int zsl_sta_weighted_median(struct zsl_vec *v, struct zsl_vec *w, zsl_real_t *m)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure the vectors dimensions match. */
	if (v->sz != w->sz) {
		return -EINVAL;
	}

	/* Make sure the weights are positive or zero. */
	zsl_real_t sum = 0.0;
	for (size_t i = 0; i < v->sz; i++) {
		sum += w->data[i];
		if (w->data[i] < 0.0) {
			return -EINVAL;
		}
	}

	/* Make sure that that the sum of the weights is 1. */
	if (ZSL_ABS(sum - 1.0) > 1E-6) {
		return -EINVAL;
	}
#endif

	ZSL_VECTOR_DEF(vsort, v->sz);
	zsl_real_t lsum = 0.0;
	size_t i = 0;
	while (lsum <= 0.5) {
		lsum += w->data[i];
		i++;
	}

	lsum -= w->data[i - 1];
	zsl_vec_init(&vsort);
	zsl_vec_sort(v, &vsort);

	if (ZSL_ABS(lsum - 0.5) < 1E-6) {
		*m = (vsort.data[i - 1] + vsort.data[i - 2]) / 2.0;
	} else {
		*m = vsort.data[i - 1];
	}

	return 0;
}

int zsl_sta_quart(struct zsl_vec *v, zsl_real_t *q1, zsl_real_t *q2,
		  zsl_real_t *q3)
{
	zsl_sta_percentile(v, 25, q1);
	zsl_sta_percentile(v, 50, q2);
	zsl_sta_percentile(v, 75, q3);

	return 0;
}

int zsl_sta_quart_range(struct zsl_vec *v, zsl_real_t *r)
{
	zsl_real_t q1, q3;

	zsl_sta_percentile(v, 25, &q1);
	zsl_sta_percentile(v, 75, &q3);

	*r = q3 - q1;

	return 0;
}

int zsl_sta_mode(struct zsl_vec *v, struct zsl_vec *w)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure w and v are the same length. */
	if (v->sz != w->sz) {
		return -EINVAL;
	}
#endif

	size_t i, j = 0, count, maxcount = 0;

	ZSL_VECTOR_DEF(u, v->sz);
	ZSL_VECTOR_DEF(w_temp, v->sz);

	zsl_vec_init(w);

	for (i = 0; i < v->sz; i++) {
		count = zsl_vec_contains(v, v->data[i], 1E-7);
		u.data[i] = count;
		if (count > maxcount) {
			maxcount = count;
		}
	}

	for (i = 0; i < v->sz; i++) {
		if (u.data[i] == maxcount) {
			w_temp.data[j] = v->data[i];
			j++;
		}
	}

	w_temp.sz = j;
	count = 0;

	for (i = 0; i < j; i++) {
		if (w_temp.data[i] >= 1E-5 || w_temp.data[i] <= 1E-5) {
			if (zsl_vec_contains(w, w_temp.data[i], 1E-5) == 0) {
				w->data[count] = w_temp.data[i];
				count++;
			}
		}
	}

	if (zsl_vec_contains(&w_temp, 0.0, 1E-5) > 0) {
		count++;
	}

	w->sz = count;

	return 0;
}

int zsl_sta_data_range(struct zsl_vec *v, zsl_real_t *r)
{
	ZSL_VECTOR_DEF(w, v->sz);

	zsl_vec_sort(v, &w);

	*r = w.data[v->sz - 1] - w.data[0];

	return 0;
}

int zsl_sta_mean_abs_dev(struct zsl_vec *v, zsl_real_t *m)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure v has at least dimension 1. */
	if (v->sz == 0) {
		return -EINVAL;
	}
#endif
	ZSL_VECTOR_DEF(vdm, v->sz);
	zsl_sta_demean(v, &vdm);

	zsl_real_t sum = 0.0;
	for (size_t i = 0; i < v->sz; i++) {
		sum += ZSL_ABS(vdm.data[i]);
	}

	*m = sum / v->sz;

	return 0;
}

int zsl_sta_median_abs_dev(struct zsl_vec *v, zsl_real_t *m)
{
	ZSL_VECTOR_DEF(med, v->sz);
	zsl_real_t median;

	zsl_sta_median(v, &median);
	for (size_t i = 0; i < v->sz; i++) {
		med.data[i] = ZSL_ABS(v->data[i] - median);
	}

	zsl_sta_median(&med, m);

	return 0;
}

int zsl_sta_var(struct zsl_vec *v, zsl_real_t *var)
{
	ZSL_VECTOR_DEF(w, v->sz);
	*var = 0;

	zsl_sta_demean(v, &w);

	for (size_t i = 0; i < v->sz; i++) {
		*var += w.data[i] * w.data[i];
	}

	*var /= v->sz - 1;

	return 0;
}

int zsl_sta_std_dev(struct zsl_vec *v, zsl_real_t *s)
{
	zsl_real_t var;

	zsl_sta_var(v, &var);

	*s = ZSL_SQRT(var);

	return 0;
}

int zsl_sta_covar(struct zsl_vec *v, struct zsl_vec *w, zsl_real_t *c)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure v and w are equal length. */
	if (v->sz != w->sz) {
		return -EINVAL;
	}
#endif

	ZSL_VECTOR_DEF(v_dm, v->sz);
	ZSL_VECTOR_DEF(w_dm, w->sz);

	zsl_sta_demean(v, &v_dm);
	zsl_sta_demean(w, &w_dm);

	*c = 0;
	for (size_t i = 0; i < v->sz; i++) {
		*c += v_dm.data[i] * w_dm.data[i];
	}

	*c /= v->sz - 1;

	return 0;
}

int zsl_sta_covar_mtx(struct zsl_mtx *m, struct zsl_mtx *mc)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure 'mc' is a square matrix with same num. columns as 'm'. */
	if (mc->sz_rows != mc->sz_cols || mc->sz_cols != m->sz_cols) {
		return -EINVAL;
	}
#endif

	zsl_real_t c;

	ZSL_VECTOR_DEF(v1, m->sz_rows);
	ZSL_VECTOR_DEF(v2, m->sz_rows);

	for (size_t i = 0; i < m->sz_cols; i++) {
		for (size_t j = 0; j < m->sz_cols; j++) {
			zsl_mtx_get_col(m, i, v1.data);
			zsl_mtx_get_col(m, j, v2.data);
			zsl_sta_covar(&v1, &v2, &c);
			mc->data[(i * m->sz_cols) + j] = c;
		}
	}

	return 0;
}

int zsl_sta_linear_reg(struct zsl_vec *x, struct zsl_vec *y,
		       struct zsl_sta_linreg *c)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure x and y are equal length. */
	if (x->sz != y->sz) {
		return -EINVAL;
	}
#endif

	zsl_real_t sumx, sumy, sumxy, sumxx, sumyy;

	sumx = sumy = sumxy = sumxx = sumyy = 0.0;

	for (size_t i = 0; i < x->sz; i++) {
		sumx += x->data[i];
		sumy += y->data[i];
		sumxy += x->data[i] * y->data[i];
		sumxx += x->data[i] * x->data[i];
		sumyy += y->data[i] * y->data[i];
	}

	c->slope = (x->sz * sumxy - sumx * sumy) / (x->sz * sumxx - sumx * sumx);
	c->intercept = (sumy - c->slope * sumx) / x->sz;
	c->correlation = (x->sz * sumxy - sumx * sumy) /
			 ZSL_SQRT((x->sz * sumxx - sumx * sumx) *
				  (x->sz * sumyy - sumy * sumy));

	return 0;
}

#ifndef CONFIG_ZSL_SINGLE_PRECISION
int zsl_sta_mult_linear_reg(struct zsl_mtx *x, struct zsl_vec *y,
			    struct zsl_vec *b, zsl_real_t *r)
{

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure matrices and vectors' sizes match. */
	if (x->sz_rows != y->sz || x->sz_cols + 1 != b->sz) {
		return -EINVAL;
	}
#endif

	ZSL_MATRIX_DEF(x_exp, x->sz_rows, (x->sz_cols + 1));
	ZSL_MATRIX_DEF(x_trans, (x->sz_cols + 1), x->sz_rows);
	ZSL_MATRIX_DEF(xx, (x->sz_cols + 1), (x->sz_cols + 1));
	ZSL_MATRIX_DEF(xinv, (x->sz_cols + 1), (x->sz_cols + 1));
	ZSL_MATRIX_DEF(ymtx, y->sz, 1);
	ZSL_MATRIX_DEF(xtemp, (x->sz_cols + 1), x->sz_rows);
	ZSL_MATRIX_DEF(bmtx, (x->sz_cols + 1), 1);
	ZSL_MATRIX_DEF(xtemp2, x->sz_rows, 1);
	ZSL_MATRIX_DEF(emtx, x->sz_rows, 1);

	zsl_real_t v[x->sz_rows];
	zsl_mtx_init(&x_exp, NULL);
	for (size_t i = 0; i < x->sz_rows; i++) {
		v[i] = 1.;
	}

	zsl_mtx_set_col(&x_exp, 0, v);
	for (size_t j = 0; j < x->sz_cols; j++) {
		zsl_mtx_get_col(x, j, v);
		zsl_mtx_set_col(&x_exp, j + 1, v);
	}

	zsl_mtx_trans(&x_exp, &x_trans);
	zsl_mtx_mult(&x_trans, &x_exp, &xx);

	zsl_real_t det;
	zsl_mtx_deter(&xx, &det);
	if (ZSL_ABS(det) < 1E-6) {
		/*
		 * Currently limited to square matrices. pinv could be used,
		 * but is too resource-intensive to add at the momemt.
		 */
		return -EINVAL;
	} else {
		zsl_mtx_inv(&xx, &xinv);
	}

	zsl_mtx_from_arr(&ymtx, y->data);
	zsl_mtx_mult(&xinv, &x_trans, &xtemp);
	zsl_mtx_mult(&xtemp, &ymtx, &bmtx);
	zsl_vec_from_arr(b, bmtx.data);

	zsl_mtx_mult(&x_exp, &bmtx, &xtemp2);
	zsl_mtx_sub(&ymtx, &xtemp2, &emtx);
	zsl_real_t e_norm = 0.0, ymean, ysum = 0.0;
	zsl_sta_mean(y, &ymean);
	for (size_t g = 0; g < x->sz_rows; g++) {
		e_norm += emtx.data[g] * emtx.data[g];
		ysum += (y->data[g] - ymean) * (y->data[g] - ymean);
	}

	*r = 1. - e_norm / ysum;

	return 0;
}
#endif

#ifndef CONFIG_ZSL_SINGLE_PRECISION
int zsl_sta_weighted_mult_linear_reg(struct zsl_mtx *x, struct zsl_vec *y,
				     struct zsl_vec *w, struct zsl_vec *b, zsl_real_t *r)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure matrices and vectors' sizes match. */
	if (x->sz_rows != y->sz || x->sz_cols + 1 != b->sz || x->sz_rows != w->sz) {
		return -EINVAL;
	}
	/* Make sure no weight is zero. */
	for (size_t k = 0; k < w->sz; k++) {
		if (ZSL_ABS(w->data[k]) < 1E-6) {
			return -EINVAL;
		}
	}
#endif

	ZSL_MATRIX_DEF(x_exp, x->sz_rows, (x->sz_cols + 1));
	ZSL_MATRIX_DEF(x_trans, (x->sz_cols + 1), x->sz_rows);
	ZSL_MATRIX_DEF(xw, (x->sz_cols + 1), x->sz_rows);
	ZSL_MATRIX_DEF(xx, (x->sz_cols + 1), (x->sz_cols + 1));
	ZSL_MATRIX_DEF(xinv, (x->sz_cols + 1), (x->sz_cols + 1));
	ZSL_MATRIX_DEF(ymtx, y->sz, 1);
	ZSL_MATRIX_DEF(xtemp, (x->sz_cols + 1), x->sz_rows);
	ZSL_MATRIX_DEF(bmtx, (x->sz_cols + 1), 1);
	ZSL_MATRIX_DEF(xtemp2, x->sz_rows, 1);
	ZSL_MATRIX_DEF(emtx, x->sz_rows, 1);
	ZSL_MATRIX_DEF(idx, w->sz, w->sz);

	zsl_mtx_init(&idx, zsl_mtx_entry_fn_identity);
	for (size_t k = 0; k < w->sz; k++) {
		w->data[k] = 1. / w->data[k];
		zsl_mtx_scalar_mult_row_d(&idx, k, w->data[k]);
	}

	zsl_real_t v[x->sz_rows];
	zsl_mtx_init(&x_exp, NULL);
	for (size_t i = 0; i < x->sz_rows; i++) {
		v[i] = 1.;
	}

	zsl_mtx_set_col(&x_exp, 0, v);
	for (size_t j = 0; j < x->sz_cols; j++) {
		zsl_mtx_get_col(x, j, v);
		zsl_mtx_set_col(&x_exp, j + 1, v);
	}

	zsl_mtx_trans(&x_exp, &x_trans);
	zsl_mtx_mult(&x_trans, &idx, &xw);
	zsl_mtx_mult(&xw, &x_exp, &xx);

	zsl_real_t det;
	zsl_mtx_deter(&xx, &det);
	if (ZSL_ABS(det) < 1E-6) {
		/*
		 * Currently limited to square matrices. pinv could be used,
		 * but is too resource-intensive to add at the momemt.
		 */
		return -EINVAL;
	} else {
		zsl_mtx_inv(&xx, &xinv);
	}

	zsl_mtx_from_arr(&ymtx, y->data);
	zsl_mtx_mult(&xinv, &xw, &xtemp);
	zsl_mtx_mult(&xtemp, &ymtx, &bmtx);
	zsl_vec_from_arr(b, bmtx.data);

	zsl_mtx_mult(&x_exp, &bmtx, &xtemp2);
	zsl_mtx_sub(&ymtx, &xtemp2, &emtx);
	zsl_real_t e_norm = 0.0, ymean, ysum = 0.0;
	zsl_sta_mean(y, &ymean);
	for (size_t g = 0; g < x->sz_rows; g++) {
		e_norm += emtx.data[g] * emtx.data[g];
		ysum += (y->data[g] - ymean) * (y->data[g] - ymean);
	}

	*r = 1. - e_norm / ysum;

	return 0;
}
#endif

#ifndef CONFIG_ZSL_SINGLE_PRECISION
int zsl_sta_quad_fit(struct zsl_mtx *m, struct zsl_vec *b)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure matrices and vectors' sizes are adequate. */
	if (m->sz_cols != 3 || b->sz != 9) {
		return -EINVAL;
	}
#endif

	ZSL_MATRIX_DEF(x, m->sz_rows, 9);
	ZSL_VECTOR_DEF(xv, 9);
	ZSL_VECTOR_DEF(mv, 3);
	ZSL_MATRIX_DEF(y, m->sz_rows, 1);

	for (size_t i = 0; i < m->sz_rows; i++) {
		zsl_mtx_get_row(m, i, mv.data);
		xv.data[0] = mv.data[0] * mv.data[0];
		xv.data[1] = mv.data[1] * mv.data[1];
		xv.data[2] = mv.data[2] * mv.data[2];
		xv.data[3] = 2.0 * mv.data[0] * mv.data[1];
		xv.data[4] = 2.0 * mv.data[0] * mv.data[2];
		xv.data[5] = 2.0 * mv.data[1] * mv.data[2];
		xv.data[6] = 2.0 * mv.data[0];
		xv.data[7] = 2.0 * mv.data[1];
		xv.data[8] = 2.0 * mv.data[2];
		zsl_mtx_set_row(&x, i, xv.data);
		y.data[i] = 1.0;
	}

	ZSL_MATRIX_DEF(xt, 9, m->sz_rows);
	ZSL_MATRIX_DEF(xtx, 9, 9);
	ZSL_MATRIX_DEF(inv, 9, 9);
	ZSL_MATRIX_DEF(xtmp, 9, (m->sz_rows));
	ZSL_MATRIX_DEF(btmp, 9, 1);

	zsl_mtx_trans(&x, &xt);
	zsl_mtx_mult(&xt, &x, &xtx);
	zsl_mtx_inv(&xtx, &inv);
	zsl_mtx_mult(&inv, &xt, &xtmp);
	zsl_mtx_mult(&xtmp, &y, &btmp);

	zsl_vec_from_arr(b, btmp.data);

	return 0;
}
#endif

int zsl_sta_abs_err(zsl_real_t *val, zsl_real_t *exp_val, zsl_real_t *err)
{
	*err = ZSL_ABS(*val - *exp_val);

	return 0;
}

int zsl_sta_rel_err(zsl_real_t *val, zsl_real_t *exp_val, zsl_real_t *err)
{
	*err = ZSL_ABS(100. * *val - 100. * *exp_val) / *exp_val;

	return 0;
}

int zsl_sta_sta_err(struct zsl_vec *v, zsl_real_t *err)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure v has at least dimension 1. */
	if (v->sz == 0) {
		return -EINVAL;
	}
#endif

	zsl_real_t var;
	zsl_sta_var(v, &var);
	*err = ZSL_SQRT(var / v->sz);

	return 0;
}
