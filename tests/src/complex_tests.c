/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <complex.h>    /* C99 complex number support. */
#include <zephyr/ztest.h>
#include <zsl/zsl.h>
#include "floatcheck.h"

/**
 * For details on complex number support in C99 andd C11 see;
 * https://en.wikichip.org/wiki/c/complex.h
 */

#ifdef __STDC_NO_COMPLEX__
#error "C99 Complex number support not available."
#endif

ZTEST(zsl_tests, test_complex_add)
{
	/* NOTE: zsl_real_t typedef can't be used with C99 complex numbers. */
#if CONFIG_ZSL_SINGLE_PRECISION
	float complex a = 1.0 + 0.4 * I;
	float complex b = 1.0 + 0.5 * I;
	float complex c = 0.0;
#else
	double complex a = 1.0 + 0.4 * (double)I;
	double complex b = 1.0 + 0.5 * (double)I;
	double complex c = 0.0;
#endif

	c = a + b;
	zassert_true(val_is_equal(creal(c), creal(a) + creal(b), 1E-5), NULL);
	zassert_true(val_is_equal(cimag(c), cimag(a) + cimag(b), 1E-5), NULL);
}
