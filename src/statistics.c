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

int zsl_sta_demean(struct zsl_vec *v, struct zsl_vec *w)
{
	zsl_real_t m;

	zsl_vec_copy(w, v);
	zsl_sta_mean(v, &m);
	zsl_vec_scalar_add(w, -m);

	return 0;
}

int zsl_sta_percentile(struct zsl_vec *v, size_t p, zsl_real_t *val)
{
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
	size_t i, j = 0, count, maxcount = 0;

	ZSL_VECTOR_DEF(u, v->sz);
	ZSL_VECTOR_DEF(w_temp, v->sz);

	w->sz = v->sz;
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

int zsl_sta_sta_dev(struct zsl_vec *v, zsl_real_t *s)
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

int zsl_sta_linear_reg(struct zsl_vec *v, struct zsl_vec *w,
		       struct zsl_sta_linreg *c)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure v and w are equal length. */
	if (v->sz != w->sz) {
		return -EINVAL;
	}
#endif

	zsl_real_t sumx, sumy, sumxy, sumxx, sumyy;

	sumx = sumy = sumxy = sumxx = sumyy = 0.0;

	for (size_t i = 0; i < v->sz; i++) {
		sumx += v->data[i];
		sumy += w->data[i];
		sumxy += v->data[i] * w->data[i];
		sumxx += v->data[i] * v->data[i];
		sumyy += w->data[i] * w->data[i];
	}

	c->slope = (v->sz * sumxy - sumx * sumy) / (v->sz * sumxx - sumx * sumx);
	c->intercept = (sumy - c->slope * sumx) / v->sz;
	c->correlation = (v->sz * sumxy - sumx * sumy) /
			 ZSL_SQRT((v->sz * sumxx - sumx * sumx) *
				  (v->sz * sumyy - sumy * sumy));

	return 0;
}

int zsl_sta_abs_err(zsl_real_t *val, zsl_real_t *exp_val, zsl_real_t *err)
{
	*err = ZSL_ABS(*val - *exp_val);

	return 0;
}

int zsl_sta_rel_err(zsl_real_t *val, zsl_real_t *exp_val, zsl_real_t *err)
{
	*err = ZSL_ABS(100. * *val - 100 * *exp_val) / *exp_val;

	return 0;
}
