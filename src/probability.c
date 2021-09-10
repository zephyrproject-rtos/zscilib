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
#include <zsl/probability.h>

zsl_real_t zsl_prob_uni_pdf(zsl_real_t *a, zsl_real_t *b, zsl_real_t *x)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure b is bigger than a. */
	if (*a >= *b) {
		return -EINVAL;
	}
#endif

	if (*x >= *a && *x <= *b) {
		return 1. / (*b - *a);
	} else {
		return 0.0;
	}
}

int zsl_prob_uni_mean(zsl_real_t *a, zsl_real_t *b, zsl_real_t *m)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure b is bigger than a. */
	if (*a >= *b) {
		return -EINVAL;
	}
#endif

	*m = 0.5 * (*a + *b);

	return 0;
}

int zsl_prob_uni_var(zsl_real_t *a, zsl_real_t *b, zsl_real_t *v)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure b is bigger than a. */
	if (*a >= *b) {
		return -EINVAL;
	}
#endif

	*v = (*b - *a) * (*b - *a) / 12.0;

	return 0;
}

zsl_real_t zsl_prob_uni_cdf(zsl_real_t *a, zsl_real_t *b, zsl_real_t *x)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure b is bigger than a. */
	if (*a >= *b) {
		return -EINVAL;
	}
#endif

	if (*x >= *a && *x <= *b) {
		return (*x - *a) / (*b - *a);
	} else if (*x < *a) {
		return 0.0;
	} else {
		return 1.0;
	}
}

zsl_real_t zsl_prob_normal_pdf(zsl_real_t *m, zsl_real_t *s, zsl_real_t *x)
{
	zsl_real_t y;

	y = (1. / (*s * ZSL_SQRT(2. * ZSL_PI))) *
	    ZSL_POW(ZSL_E, -0.5 * ((*x - *m) / *s) * ((*x - *m) / *s));

	return y;
}

zsl_real_t zsl_prob_normal_cdf(zsl_real_t *m, zsl_real_t *s, zsl_real_t *x)
{
	zsl_real_t y;

	y = 0.5 * (1 + ZSL_ERF((*x - *m) / (*s * ZSL_SQRT(2.))));

	return y;
}

zsl_real_t zsl_prob_erf_inv(zsl_real_t *x)
{
	zsl_real_t p, r, t;

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure x is between -1 and 1. */
	if (*x <= -1.0 || *x >= 1.0) {
		return -EINVAL;
	}
#endif

	t = ZSL_FMA(*x, 0.0 - *x, 1.0);
	t = ZSL_LOG(t);
	if (ZSL_ABS(t) > 6.125) {
		p = 3.03697567e-10;                     //  0x1.4deb44p-32
		p = ZSL_FMA(p, t,  2.93243101e-8);      //  0x1.f7c9aep-26
		p = ZSL_FMA(p, t,  1.22150334e-6);      //  0x1.47e512p-20
		p = ZSL_FMA(p, t,  2.84108955e-5);      //  0x1.dca7dep-16
		p = ZSL_FMA(p, t,  3.93552968e-4);      //  0x1.9cab92p-12
		p = ZSL_FMA(p, t,  3.02698812e-3);      //  0x1.8cc0dep-9
		p = ZSL_FMA(p, t,  4.83185798e-3);      //  0x1.3ca920p-8
		p = ZSL_FMA(p, t, -2.64646143e-1);      // -0x1.0eff66p-2
		p = ZSL_FMA(p, t,  8.40016484e-1);      //  0x1.ae16a4p-1
	} else {                                        // max ulp error = 2.35002
		p = 5.43877832e-9;                      //  0x1.75c000p-28
		p = ZSL_FMA(p, t,  1.43285448e-7);      //  0x1.33b402p-23
		p = ZSL_FMA(p, t,  1.22774793e-6);      //  0x1.499232p-20
		p = ZSL_FMA(p, t,  1.12963626e-7);      //  0x1.e52cd2p-24
		p = ZSL_FMA(p, t, -5.61530760e-5);      // -0x1.d70bd0p-15
		p = ZSL_FMA(p, t, -1.47697632e-4);      // -0x1.35be90p-13
		p = ZSL_FMA(p, t,  2.31468678e-3);      //  0x1.2f6400p-9
		p = ZSL_FMA(p, t,  1.15392581e-2);      //  0x1.7a1e50p-7
		p = ZSL_FMA(p, t, -2.32015476e-1);      // -0x1.db2aeep-3
		p = ZSL_FMA(p, t,  8.86226892e-1);
	}
	r = *x * p;
	return r;
}

zsl_real_t zsl_prob_normal_cdf_inv(zsl_real_t *m, zsl_real_t *s, zsl_real_t *p)
{
	zsl_real_t x, y;

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure p is between 0 and 1. */
	if (*p <= 0.0 || *p >= 1.0) {
		return -EINVAL;
	}
#endif

	x = 2.0 * *p - 1.0;
	y = *m + *s * ZSL_SQRT(2.0) * zsl_prob_erf_inv(&x);

	return y;
}

int zsl_prob_factorial(int *n)
{
	if (*n == 0 || *n == 1) {
		return 1;
	} else if (*n < 0) {
		return -EINVAL;
	}

	int n2 = *n;

	for (int i = 1; i < *n; i++) {
		n2 *= (*n - i);
	}

	return n2;
}

int zsl_prob_binomial_coef(int *n, int *k, int *c)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure n is positive or zero. */
	if (*n < 0) {
		return -EINVAL;
	}
#endif

	if (*k > *n || *k < 0) {
		*c = 0;
		return 0;
	}

	int kn = *n - *k;
	*c = zsl_prob_factorial(n) /
	     (zsl_prob_factorial(k) * zsl_prob_factorial(&kn));

	return 0;
}

zsl_real_t zsl_prob_binomial_pdf(int *n, zsl_real_t *p, int *x)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure p is between 0 and 1. */
	if (*p > 1.0 || *p < 0.0) {
		return -EINVAL;
	}
	/* Make sure n is positive or zero. */
	if (*n < 0) {
		return -EINVAL;
	}
#endif

	int c;
	zsl_prob_binomial_coef(n, x, &c);

	return c * ZSL_POW(*p, *x) * ZSL_POW((1. - *p), (*n - *x));
}

int zsl_prob_binomial_mean(int *n, zsl_real_t *p, zsl_real_t *m)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure p is between 0 and 1. */
	if (*p > 1.0 || *p < 0.0) {
		return -EINVAL;
	}
	/* Make sure n is positive or zero. */
	if (*n < 0) {
		return -EINVAL;
	}
#endif

	*m = *n * *p;

	return 0;
}

int zsl_prob_binomial_var(int *n, zsl_real_t *p, zsl_real_t *v)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure p is between 0 and 1. */
	if (*p > 1.0 || *p < 0.0) {
		return -EINVAL;
	}
	/* Make sure n is positive or zero. */
	if (*n < 0) {
		return -EINVAL;
	}
#endif

	*v = *n * *p * (1. - *p);

	return 0;
}

zsl_real_t zsl_prob_binomial_cdf(int *n, zsl_real_t *p, int *x)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure p is between 0 and 1. */
	if (*p > 1.0 || *p < 0.0) {
		return -EINVAL;
	}
	/* Make sure n is positive or zero. */
	if (*n < 0) {
		return -EINVAL;
	}
#endif
	if (*x < 0) {
		return 0;
	}

	zsl_real_t y = 0.0;
	int c;

	for (int i = 0; i <= *x; i++) {
		zsl_prob_binomial_coef(n, &i, &c);
		y += c * ZSL_POW(*p, i) * ZSL_POW((1. - *p), (*n - i));
	}


	return y;
}



int zsl_prob_entropy(struct zsl_vec *v, zsl_real_t *h)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure the sum of all coefficients in v is 1 and that each
	 * probability is greater or equal than 0. */
	zsl_real_t sum = -1.0;
	for (size_t i = 0; i < v->sz; i++) {
		if (v->data[i] < 0.0) {
			return -EINVAL;
		}
		sum += v->data[i];
	}
	if (ZSL_ABS(sum) > 1E-6) {
		return -EINVAL;
	}
#endif

	*h = 0.0;
	for (size_t i = 0; i < v->sz; i++) {
		*h -= v->data[i] * ZSL_LOG(v->data[i]);
	}

	*h /= ZSL_LOG(2.);

	return 0;
}

int zsl_prob_bayes(zsl_real_t *pa, zsl_real_t *pb, zsl_real_t *pba,
		   zsl_real_t *pab)
{

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure every probability is between 0 and 1, and pb is not zero. */
	if (*pa < 0.0 || *pa > 1.0) {
		return -EINVAL;
	}
	if (*pb <= 0.0 || *pb > 1.0 || *pb < *pa * *pba) {
		return -EINVAL;
	}
	if (*pba < 0.0 || *pba > 1.0) {
		return -EINVAL;
	}
#endif

	*pab = (*pba * *pa) / *pb;

	return 0;
}
