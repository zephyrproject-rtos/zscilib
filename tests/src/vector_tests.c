/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>
#include <zsl/zsl.h>
#include <zsl/vectors.h>
#include "floatcheck.h"

ZTEST(zsl_tests, test_vector_init)
{
	int rc;

	ZSL_VECTOR_DEF(v, 3);

	/* Initialize the vector 'v'. */
	rc = zsl_vec_init(&v);
	zassert_true(rc == 0, NULL);

	/* Test values. */
	for (size_t i = 0; i < v.sz; i++) {
		zassert_true(v.data[i] == 0.0, NULL);
	}
}

/**
 * @brief zsl_vec_from_arr unit tests.
 *
 * This test verifies the zsl_vec_from_arr function.
 */
ZTEST(zsl_tests, test_vector_from_arr)
{
	int rc;
	zsl_real_t x;

	/* Destination vector. */
	ZSL_VECTOR_DEF(v, 3);

	/* Test array. */
	zsl_real_t a[3] = { 0.0, 1.0, 2.0 };

	/* Assign the array a to vector 'v'. */
	rc = zsl_vec_from_arr(&v, a);
	zassert_true(rc == 0, NULL);

	/* Test values. */
	for (size_t i = 0; i < v.sz; i++) {
		x = v.data[i];
		zassert_true(x == a[i], NULL);
	}
}

ZTEST(zsl_tests, test_vector_copy)
{
	int rc;

	/* Define two vectors. */
	ZSL_VECTOR_DEF(v, 4);
	ZSL_VECTOR_DEF(vcopy, 3);

	/* Initialize 'vcopy' and define 'v'. */
	zsl_real_t a[4] = { 4.0, 5.0, 3.0, 2.0 };

	rc = zsl_vec_from_arr(&v, a);
	zassert_true(rc == 0, NULL);

	rc = zsl_vec_init(&vcopy);
	zassert_true(rc == 0, NULL);

	/* Copy 'v' into 'vcopy'. */
	rc = zsl_vec_copy(&vcopy, &v);
	zassert_true(rc == 0, NULL);

	/* Check dimension. */
	zassert_true(v.sz == vcopy.sz, NULL);

	/* Chack values. */
// 	zassert_equal(vcopy.data[0], v.data[0], NULL);
	zassert_equal(vcopy.data[1], v.data[1], NULL);
	zassert_equal(vcopy.data[2], v.data[2], NULL);
	zassert_equal(vcopy.data[3], v.data[3], NULL);
}

ZTEST(zsl_tests, test_vector_get_subset)
{
	int rc;

	ZSL_VECTOR_DEF(vsrc, 10);
	ZSL_VECTOR_DEF(vcp, 6);

	zsl_real_t a[10] = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0 };

	/* Populate the source vector. */
	rc = zsl_vec_from_arr(&vsrc, a);
	zassert_true(rc == 0, NULL);

	/* Get a valid subset (no range issues). */
	rc = zsl_vec_get_subset(&vsrc, 3, 6, &vcp);
	zassert_true(rc == 0, NULL);
	zassert_equal(vcp.sz, 6, NULL);
	zassert_equal(vcp.data[0], a[3], NULL);
	zassert_equal(vcp.data[1], a[4], NULL);
	zassert_equal(vcp.data[2], a[5], NULL);
	zassert_equal(vcp.data[3], a[6], NULL);
	zassert_equal(vcp.data[4], a[7], NULL);
	zassert_equal(vcp.data[5], a[8], NULL);

	/* vcp->sz resize test (sz should truncate from 6 to 3). */
	zsl_vec_init(&vcp);
	rc = zsl_vec_get_subset(&vsrc, 3, 3, &vcp);
	zassert_true(rc == 0, NULL);
	zassert_equal(vcp.sz, 3, NULL);
	zassert_equal(vcp.data[0], a[3], NULL);
	zassert_equal(vcp.data[1], a[4], NULL);
	zassert_equal(vcp.data[2], a[5], NULL);

	/* Check for len overrun truncate (sz should truncate to 1). */
	vcp.sz = 6;
	zsl_vec_init(&vcp);
	rc = zsl_vec_get_subset(&vsrc, 9, 3, &vcp);
	zassert_true(rc == 0, NULL);
	zassert_equal(vcp.sz, 1, NULL);
	zassert_equal(vcp.data[0], a[9], NULL);

	/* Check to out of range error. */
	vcp.sz = 6;
	zsl_vec_init(&vcp);
	rc = zsl_vec_get_subset(&vsrc, 10, 3, &vcp);
	zassert_true(rc == -EINVAL, NULL);

	/* Check buffer too small error (needs 3 elements, has 2). */
	vcp.sz = 2;
	zsl_vec_init(&vcp);
	rc = zsl_vec_get_subset(&vsrc, 3, 3, &vcp);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_vector_add)
{
	int rc;

	ZSL_VECTOR_DEF(va, 3);
	ZSL_VECTOR_DEF(vb, 3);
	ZSL_VECTOR_DEF(vc, 3);

	zsl_real_t a[3] = { 0.0, 1.0, 2.0 };
	zsl_real_t b[3] = { -1.0, 0.5, 0.1 };
	zsl_real_t c[3] = { -1.0, 1.5, 2.1 };

	/* Assign arrays to vectors. */
	rc = zsl_vec_from_arr(&va, a);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&vb, b);
	zassert_true(rc == 0, NULL);

	/* Perform the add operation. */
	rc = zsl_vec_add(&va, &vb, &vc);
	zassert_true(rc == 0, NULL);
	zassert_equal(vc.data[0], c[0], NULL);
	zassert_equal(vc.data[1], c[1], NULL);
	zassert_equal(vc.data[2], c[2], NULL);
}

ZTEST(zsl_tests, test_vector_sub)
{
	int rc;

	ZSL_VECTOR_DEF(va, 3);
	ZSL_VECTOR_DEF(vb, 3);
	ZSL_VECTOR_DEF(vc, 3);

	zsl_real_t a[3] = { 0.0, 1.0, 2.0 };
	zsl_real_t b[3] = { -1.0, 0.5, 0.1 };
	zsl_real_t c[3] = { 1.0, 0.5, 1.9 };

	/* Assign arrays to vectors. */
	rc = zsl_vec_from_arr(&va, a);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&vb, b);
	zassert_true(rc == 0, NULL);

	/* Perform the add operation. */
	rc = zsl_vec_sub(&va, &vb, &vc);
	zassert_true(rc == 0, NULL);
	zassert_equal(vc.data[0], c[0], NULL);
	zassert_equal(vc.data[1], c[1], NULL);
	zassert_equal(vc.data[2], c[2], NULL);
}

ZTEST(zsl_tests, test_vector_neg)
{
	int rc;

	ZSL_VECTOR_DEF(v, 3);

	zsl_real_t a[3] = {  0.0,  1.0,  2.0 };
	zsl_real_t b[3] = { -0.0, -1.0, -2.0 };

	/* Assign array to vector. */
	rc = zsl_vec_from_arr(&v, a);
	zassert_true(rc == 0, NULL);

	/* Perform the add operation. */
	rc = zsl_vec_neg(&v);
	zassert_true(rc == 0, NULL);
	zassert_equal(v.data[0], b[0], NULL);
	zassert_equal(v.data[1], b[1], NULL);
	zassert_equal(v.data[2], b[2], NULL);
}

ZTEST(zsl_tests, test_vector_sum)
{
	int rc;

	ZSL_VECTOR_DEF(va, 3);
	ZSL_VECTOR_DEF(vb, 3);
	ZSL_VECTOR_DEF(vc, 3);
	ZSL_VECTOR_DEF(vsum, 3);

	/* Set all vectors to zero. */
	zsl_vec_init(&va);
	zsl_vec_init(&vb);
	zsl_vec_init(&vc);
	zsl_vec_init(&vsum);

	struct zsl_vec *vlist[3] = { &va, &vb, &vc };

	/* Assign values to vectors. */
	va.data[0] = 1.0;
	va.data[1] = 2.0;
	va.data[2] = 3.0;
	vb.data[0] = 3.0;
	vb.data[1] = 2.0;
	vb.data[2] = 1.0;
	vc.data[0] = -1.0;
	vc.data[1] = -2.0;
	vc.data[2] = -3.0;

	/* Perform sum operation on va..vc. */
	rc = zsl_vec_sum(vlist, 3, &vsum);
	zassert_true(rc == 0, NULL);

	/* Check output. */
	for (size_t i = 0; i < 3; i++) {
		zassert_equal(vsum.data[i], va.data[i] + vb.data[i] +
			      vc.data[i], NULL);
	}
}

ZTEST(zsl_tests, test_vector_scalar_add)
{
	int rc;
	zsl_real_t s = 0.3;

	ZSL_VECTOR_DEF(v, 4);
	zsl_real_t vi[4] = { 5.1, -3.4, 0.7, -8.1 };

	/* Assign array to vector. */
	rc = zsl_vec_from_arr(&v, vi);
	zassert_true(rc == 0, NULL);

	/* Add 0.3 to every component of 'v'. */
	rc = zsl_vec_scalar_add(&v, s);
	zassert_true(rc == 0, NULL);

	/* Check output. */
	zassert_true(val_is_equal(v.data[0], 5.4, 1E-6), NULL);
	zassert_true(val_is_equal(v.data[1], -3.1, 1E-6), NULL);
	zassert_true(val_is_equal(v.data[2], 1.0, 1E-6), NULL);
	zassert_true(val_is_equal(v.data[3], -7.8, 1E-6), NULL);
}

ZTEST(zsl_tests, test_vector_scalar_mult)
{
	int rc;
	zsl_real_t s = 2.0;

	ZSL_VECTOR_DEF(v, 4);
	zsl_real_t vi[4] = { 3.1, -3.6, -1.6, 0.5 };

	/* Assign array to vector. */
	rc = zsl_vec_from_arr(&v, vi);
	zassert_true(rc == 0, NULL);

	/* Multiply every component of 'v' by 2. */
	rc = zsl_vec_scalar_mult(&v, s);
	zassert_true(rc == 0, NULL);

	/* Check output. */
	zassert_true(val_is_equal(v.data[0], 6.2, 1E-6), NULL);
	zassert_true(val_is_equal(v.data[1], -7.2, 1E-6), NULL);
	zassert_true(val_is_equal(v.data[2], -3.2, 1E-6), NULL);
	zassert_true(val_is_equal(v.data[3], 1.0, 1E-6), NULL);
}

ZTEST(zsl_tests, test_vector_scalar_div)
{
	int rc;
	zsl_real_t s = 0.0;

	ZSL_VECTOR_DEF(v, 4);
	zsl_real_t vi[4] = { -2.6, 5.4, 7.6, -0.3 };

	/* Assign array to vector. */
	rc = zsl_vec_from_arr(&v, vi);
	zassert_true(rc == 0, NULL);

	/* Divide every component of 'v' by 0.0. This should return an error. */
	rc = zsl_vec_scalar_div(&v, s);
	zassert_true(rc == -EINVAL, NULL);

	/* Divide now every component of 'v' by 1.3. */
	s = 1.3;
	rc = zsl_vec_scalar_div(&v, s);
	zassert_true(rc == 0, NULL);

	/* Check output. */
	zassert_true(val_is_equal(v.data[0], -2.0, 1E-6), NULL);
	zassert_true(val_is_equal(v.data[1], 4.1538461538, 1E-6), NULL);
	zassert_true(val_is_equal(v.data[2], 5.8461538461, 1E-6), NULL);
	zassert_true(val_is_equal(v.data[3], -0.2307692307, 1E-6), NULL);
}

ZTEST(zsl_tests, test_vector_dist)
{
	int rc;
	zsl_real_t dist;

	ZSL_VECTOR_DEF(v, 3);
	ZSL_VECTOR_DEF(w, 3);
	ZSL_VECTOR_DEF(u, 4);

	zsl_real_t vi[3] = { 0.0, 1.0, 2.0 };
	zsl_real_t wi[3] = { -1.0, 0.5, 0.1 };
	zsl_real_t ui[4] = { -3.2, 1.0, 4.1, 9.2 };

	/* Assign arrays to vectors. */
	rc = zsl_vec_from_arr(&v, vi);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&w, wi);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&u, ui);
	zassert_true(rc == 0, NULL);

	/* Calculate the distance between 'v' and 'w'. */
	dist = zsl_vec_dist(&v, &w);
	zassert_true(val_is_equal(dist, 2.2045407685, 1E-6), NULL);

	/* Calculate the distance between 'v' and 'u'. An error is expected
	 * due to the difference of the vectors in dimension. */
	dist = zsl_vec_dist(&v, &u);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(dist != dist, NULL);
}

ZTEST(zsl_tests, test_vector_dot)
{
	int rc;
	zsl_real_t d;

	ZSL_VECTOR_DEF(v, 3);
	ZSL_VECTOR_DEF(w, 3);
	ZSL_VECTOR_DEF(u, 4);

	zsl_real_t vi[3] = { 0.0, 1.0, 2.0 };
	zsl_real_t wi[3] = { 3.4, 2.5, 0.5 };
	zsl_real_t ui[4] = { -3.2, 1.0, 4.1, 9.2 };

	/* Assign arrays to vectors. */
	rc = zsl_vec_from_arr(&v, vi);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&w, wi);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&u, ui);
	zassert_true(rc == 0, NULL);

	/* Calculate the dot product of 'v' and 'w'. */
	rc = zsl_vec_dot(&v, &w, &d);
	zassert_true(rc == 0, NULL);

	/* Check the output. */
	zassert_true(val_is_equal(d, 3.5, 1E-6), NULL);

	/* Calculate the dot product of 'v' and 'u'. An error is expected due
	 * to the difference of the vectors in dimension. */
	rc = zsl_vec_dot(&v, &u, &d);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_vector_norm)
{
	int rc;

	ZSL_VECTOR_DEF(v, 3);
	zsl_real_t vi[3] = { -2.0, 1.5, 3.8 };
	zsl_real_t norm;

	/* Assign array to vector. */
	rc = zsl_vec_from_arr(&v, vi);
	zassert_true(rc == 0, NULL);

	/* Calculate the norm of the vector. */
	norm = zsl_vec_norm(&v);

	/* Check output. */
	zassert_true(val_is_equal(norm, 4.5486261662, 1E-6), NULL);

	/* Calculate the norm of a NULL vector. */
	norm = zsl_vec_norm(NULL);

	/* Check output. It should be zero. */
	zassert_true(val_is_equal(norm, 0.0, 1E-6), NULL);
}

ZTEST(zsl_tests, test_vector_project)
{
	int rc;

	ZSL_VECTOR_DEF(v, 3);
	ZSL_VECTOR_DEF(w, 3);
	ZSL_VECTOR_DEF(u, 4);
	ZSL_VECTOR_DEF(t, 3);

	zsl_real_t vi[3] = { 3.0, 1.0, 2.0 };
	zsl_real_t wi[3] = { 3.4, 2.5, 0.5 };
	zsl_real_t ui[4] = { -3.2, 1.0, 4.1, 9.2 };

	/* Assign arrays to vectors and initialize 't'. */
	rc = zsl_vec_from_arr(&v, vi);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&w, wi);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&u, ui);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_init(&t);
	zassert_true(rc == 0, NULL);

	/* Calculate the projection of 'v' over 'w' and place it on 't'. */
	rc = zsl_vec_project(&v, &w, &t);
	zassert_true(rc == 0, NULL);

	/* Check the output vector 't'. */
	zassert_true(val_is_equal(t.data[0], 2.9357142857, 1E-6), NULL);
	zassert_true(val_is_equal(t.data[1], 0.9785714286, 1E-6), NULL);
	zassert_true(val_is_equal(t.data[2], 1.9571428571, 1E-6), NULL);

	/* Calculate the projection of 'v' over 'w' and place it on 't'. An
	 * error is expected due to the differet dimension of the vectors. */
	rc = zsl_vec_project(&v, &u, &t);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_vector_to_unit)
{
	int rc;

	ZSL_VECTOR_DEF(v, 3);
	ZSL_VECTOR_DEF(w, 3);

	zsl_real_t vi[3] = { 4.0, 2.1, -3.5 };
	zsl_real_t wi[3] = { 0.0, 0.0, 0.0 };

	/* Assign arrays to vectors. */
	rc = zsl_vec_from_arr(&v, vi);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&w, wi);
	zassert_true(rc == 0, NULL);

	/* Make the vector 'v' an unitary one. */
	rc = zsl_vec_to_unit(&v);
	zassert_true(rc == 0, NULL);

	/* Check output. */
	zassert_true(val_is_equal(v.data[0], 0.6999256369, 1E-6), NULL);
	zassert_true(val_is_equal(v.data[1], 0.3674609593, 1E-6), NULL);
	zassert_true(val_is_equal(v.data[2], -0.6124349322, 1E-6), NULL);

	/* Make the vector 'w' an unitary one. Since 'w' has a zero norm, it
	 * will be set to (1.0, 0.0, 0.0). */
	rc = zsl_vec_to_unit(&w);
	zassert_true(rc == 0, NULL);

	/* Check output. */
	zassert_true(val_is_equal(w.data[0], 1.0, 1E-6), NULL);
	zassert_true(val_is_equal(w.data[1], 0.0, 1E-6), NULL);
	zassert_true(val_is_equal(w.data[2], 0.0, 1E-6), NULL);
}

ZTEST(zsl_tests, test_vector_cross)
{
	int rc;

	ZSL_VECTOR_DEF(v, 3);
	ZSL_VECTOR_DEF(w, 3);
	ZSL_VECTOR_DEF(u, 4);
	ZSL_VECTOR_DEF(t, 3);

	zsl_real_t vi[3] = { 3.0, 1.0, 2.0 };
	zsl_real_t wi[3] = { 3.4, 2.5, 0.5 };
	zsl_real_t ui[4] = { -3.2, 1.0, 4.1, 9.2 };

	/* Assign arrays to vectors and initialize 't'. */
	rc = zsl_vec_from_arr(&v, vi);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&w, wi);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&u, ui);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_init(&t);
	zassert_true(rc == 0, NULL);

	/* Calculate the cross product of 'v' times 'w' and place it on 't'. */
	rc = zsl_vec_cross(&v, &w, &t);
	zassert_true(rc == 0, NULL);

	/* Check the output vector 't'. */
	zassert_true(val_is_equal(t.data[0], -4.5, 1E-6), NULL);
	zassert_true(val_is_equal(t.data[1], 5.3, 1E-6), NULL);
	zassert_true(val_is_equal(t.data[2], 4.1, 1E-6), NULL);

	/* Calculate the cross product of 'v' times 'w' and place it on 't'. An
	 * error is expected due to the differet dimension of the vectors. */
	rc = zsl_vec_cross(&v, &u, &t);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_vector_sum_of_sqrs)
{
	int rc;

	ZSL_VECTOR_DEF(v, 3);
	zsl_real_t vi[3] = { -2.0, 3.5, -0.4 };
	zsl_real_t sum;

	/* Assign array to vector. */
	rc = zsl_vec_from_arr(&v, vi);
	zassert_true(rc == 0, NULL);

	/* Calculate the sum of squares of the vector 'v'. */
	sum = zsl_vec_sum_of_sqrs(&v);

	/* Check output. */
	zassert_true(val_is_equal(sum, 16.41, 1E-6), NULL);
}

// TODO: Analyse instruction fault in vector mean
// ZTEST(zsl_tests, test_vector_mean)
// {
// 	int rc;

// 	ZSL_VECTOR_DEF(va, 3);
// 	ZSL_VECTOR_DEF(vb, 3);
// 	ZSL_VECTOR_DEF(vc, 3);
// 	ZSL_VECTOR_DEF(vc2, 2);
// 	ZSL_VECTOR_DEF(var, 3);
// 	ZSL_VECTOR_DEF(var2, 4);

// 	/* Set all vectors to zero. */
// 	zsl_vec_init(&va);
// 	zsl_vec_init(&vb);
// 	zsl_vec_init(&vc);
// 	zsl_vec_init(&vc2);
// 	zsl_vec_init(&var);
// 	zsl_vec_init(&var2);

// 	struct zsl_vec *vlist[3] = { &va, &vb, &vc };
// 	struct zsl_vec *vlist2[3] = { &va, &vb, &vc2 };
// 	struct zsl_vec *vlist3[0];

// 	/* Assign values to vectors. */
// 	va.data[0] = 1.0;
// 	va.data[1] = 2.0;
// 	va.data[2] = -4.0;
// 	vb.data[0] = 3.5;
// 	vb.data[1] = -2.0;
// 	vb.data[2] = 0.5;
// 	vc.data[0] = -1.5;
// 	vc.data[1] = 1.5;
// 	vc.data[2] = -2.5;
// 	vc2.data[0] = -3.5;
// 	vc2.data[0] = 5.0;

// 	/* Perform the arithmetic mean on the components of 'va', 'vb' and 'vc'
// 	 * and place it on 'var'. */
// 	rc = zsl_vec_mean(vlist, 3, &var);
// 	zassert_true(rc == 0, NULL);

// 	/* Check the output. */
// 	zassert_true(val_is_equal(var.data[0], 1.0, 1E-6), NULL);
// 	zassert_true(val_is_equal(var.data[1], 0.5, 1E-6), NULL);
// 	zassert_true(val_is_equal(var.data[2], -2.0, 1E-6), NULL);

// 	/* Perform the arithmetic mean on the 'vlist3' array of none vectors. */
// 	rc = zsl_vec_mean(vlist3, 0, &var);
// 	zassert_true(rc == -EINVAL, NULL);

// 	/* Perform the arithmetic mean on the components of 'va', 'vb' and 'vc2'
// 	 * and place it on 'var'. An error is expected due to the different
// 	 * dimension of the vectors. */
// 	rc = zsl_vec_mean(vlist2, 3, &var);
// 	zassert_true(rc == -EINVAL, NULL);

// 	/* Perform the arithmetic mean on the components of 'va', 'vb' and 'vc'
// 	 * and place it on 'var2'. An error is expected due to the different
// 	 * dimension of the vectors. */
// 	rc = zsl_vec_mean(vlist, 3, &var2);
// 	zassert_true(rc == -EINVAL, NULL);
// }

ZTEST(zsl_tests, test_vector_ar_mean)
{
	int rc;

	ZSL_VECTOR_DEF(v, 3);
	ZSL_VECTOR_DEF(v2, 0);
	zsl_real_t vi[3] = { -2.0, 3.5, 1.5 };
	zsl_real_t mean;

	/* Assign array to vector. */
	rc = zsl_vec_from_arr(&v, vi);
	zassert_true(rc == 0, NULL);

	/* Calculate the arithmetic mean of the vector 'v'. */
	rc = zsl_vec_ar_mean(&v, &mean);
	zassert_true(rc == 0, NULL);

	/* Check the output. */
	zassert_true(val_is_equal(mean, 1.0, 1E-6), NULL);

	/* Calculate the arithmetic mean of the vector 'v2'. Since it has a
	 * size of zero, this should return an error. */
	rc = zsl_vec_ar_mean(&v2, &mean);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_vector_rev)
{
	int rc;

	ZSL_VECTOR_DEF(v, 7);

	zsl_real_t a[7] = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };

	/* Assign array to vector. */
	rc = zsl_vec_from_arr(&v, a);
	zassert_true(rc == 0, NULL);

	rc = zsl_vec_rev(&v);
	zassert_true(rc == 0, NULL);
	zassert_equal(v.data[0], 6.0, NULL);
	zassert_equal(v.data[1], 5.0, NULL);
	zassert_equal(v.data[2], 4.0, NULL);
	zassert_equal(v.data[3], 3.0, NULL);
	zassert_equal(v.data[4], 2.0, NULL);
	zassert_equal(v.data[5], 1.0, NULL);
	zassert_equal(v.data[6], 0.0, NULL);
}

ZTEST(zsl_tests, test_vector_zte)
{
	int rc;

	ZSL_VECTOR_DEF(v, 7);
	ZSL_VECTOR_DEF(w, 7);
	ZSL_VECTOR_DEF(u, 7);

	zsl_real_t a[7] = { 0.0, 0.0, 2.0, 0.1, 0.0, -5.0, 0.0 };
	zsl_real_t b[7] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	zsl_real_t c[7] = { 1.0, -2.0, 2.5, 0.5, 6.0, 5.0, -8.5 };

	/* Assign arrays to vectors. */
	rc = zsl_vec_from_arr(&v, a);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&w, b);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&u, c);
	zassert_true(rc == 0, NULL);

	rc = zsl_vec_zte(&v);
	zassert_true(rc == 0, NULL);

	/* Check the output. */
	zassert_equal(v.data[0], 2.0, NULL);
#ifdef CONFIG_ZSL_SINGLE_PRECISION
	zassert_true(val_is_equal(v.data[1], 0.1, 1E-5), NULL);
#else
	zassert_equal(v.data[1], 0.1, NULL);
#endif
	zassert_equal(v.data[2], -5.0, NULL);
	zassert_equal(v.data[3], 0.0, NULL);
	zassert_equal(v.data[4], 0.0, NULL);
	zassert_equal(v.data[5], 0.0, NULL);
	zassert_equal(v.data[6], 0.0, NULL);

	rc = zsl_vec_zte(&w);
	zassert_true(rc == 0, NULL);

	/* Check the output. */
	zassert_equal(w.data[0], 0.0, NULL);
	zassert_equal(w.data[1], 0.0, NULL);
	zassert_equal(w.data[2], 0.0, NULL);
	zassert_equal(w.data[3], 0.0, NULL);
	zassert_equal(w.data[4], 0.0, NULL);
	zassert_equal(w.data[5], 0.0, NULL);
	zassert_equal(w.data[6], 0.0, NULL);

	rc = zsl_vec_zte(&u);
	zassert_true(rc == 0, NULL);

	/* Check the output. */
	zassert_equal(u.data[0], 1.0, NULL);
	zassert_equal(u.data[1], -2.0, NULL);
	zassert_equal(u.data[2], 2.5, NULL);
	zassert_equal(u.data[3], 0.5, NULL);
	zassert_equal(u.data[4], 6.0, NULL);
	zassert_equal(u.data[5], 5.0, NULL);
	zassert_equal(u.data[6], -8.5, NULL);
}

/**
 * @brief zsl_vec_is_equal unit tests.
 *
 * This test verifies the zsl_vec_is_equal function.
 */
ZTEST(zsl_tests, test_vector_is_equal)
{
	int rc;
	bool eq;

	ZSL_VECTOR_DEF(v, 3);
	ZSL_VECTOR_DEF(w, 3);
	ZSL_VECTOR_DEF(x, 2);

	/* Set all vectors to zero. */
	zsl_vec_init(&v);
	zsl_vec_init(&w);
	zsl_vec_init(&x);

	/* Test arrays. */
	zsl_real_t a[3] = { 0.0, 1.0, 2.0 };
	zsl_real_t b[3] = { 0.0, 1.0, 2.0 };

	/* Check for unequal size. */
	eq = zsl_vec_is_equal(&v, &x, 1E-5);
	zassert_false(eq, NULL);

	/* Check equality for zero value vectors. */
	eq = zsl_vec_is_equal(&v, &w, 1E-5);
	zassert_true(eq, NULL);

	/* Assign the array a to vector v. */
	rc = zsl_vec_from_arr(&v, a);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&w, b);
	zassert_true(rc == 0, NULL);

	/* Check equality for array value vectors. */
	eq = zsl_vec_is_equal(&v, &w, 1E-5);
	zassert_true(eq, NULL);

	/* Check for inequality. */
	v.data[0] = 1.0;
	eq = zsl_vec_is_equal(&v, &w, 1E-5);
	zassert_false(eq, NULL);

	/* Check for inequality < epsilon (should still pass). */
	v.data[0] = 1.0;
	v.data[1] = 2.0;
	v.data[2] = 3.0;
	w.data[0] = 0.999999;
	w.data[1] = 2.0;
	w.data[2] = 3.0;
	eq = zsl_vec_is_equal(&v, &w, 1E-5);
	zassert_true(eq, NULL);

	/* Check for inequality > epsilon (should fail). */
	w.data[0] = 0.99998;
	eq = zsl_vec_is_equal(&v, &w, 1E-5);
	zassert_false(eq, NULL);

	/* Check for negative inequality < epsilon (should pass). */
	v.data[0] = -1.0;
	w.data[0] = -0.999999;
	eq = zsl_vec_is_equal(&v, &w, 1E-5);
	zassert_true(eq, NULL);

	/* Check for negative inequality > epsilon (should fail). */
	v.data[0] = -1.0;
	w.data[0] = -0.99998;
	eq = zsl_vec_is_equal(&v, &w, 1E-5);
	zassert_false(eq, NULL);

	/* Check for negative inequality > epsilon (should fail). */
	v.data[0] = -1.0;
	w.data[0] = -1.00001;
	eq = zsl_vec_is_equal(&v, &w, 1E-5);
	zassert_false(eq, NULL);
}

ZTEST(zsl_tests, test_vector_is_nonneg)
{
	int rc;
	bool neg;

	ZSL_VECTOR_DEF(v, 5);
	ZSL_VECTOR_DEF(w, 5);

	zsl_real_t a[5] = { 3.0, 4.5, -3.5, 0.0, -1.0 };
	zsl_real_t b[5] = { 4.0, 6.0, 1.5, 0.0, 3.5 };

	/* Assign arrays to vectors. */
	rc = zsl_vec_from_arr(&v, a);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&w, b);
	zassert_true(rc == 0, NULL);

	/* Check if any value is negative in 'v'. */
	neg = zsl_vec_is_nonneg(&v);
	zassert_false(neg, NULL);

	/* Check if any value is negative in 'w'. */
	neg = zsl_vec_is_nonneg(&w);
	zassert_true(neg, NULL);
}

ZTEST(zsl_tests, test_vector_contains)
{
	int count;

	ZSL_VECTOR_DEF(v, 5);

	/* Set all vectors to zero. */
	zsl_vec_init(&v);

	/* Test arrays. */
	v.data[0] = 0.0;
	v.data[1] = 1.0;
	v.data[2] = 2.0;
	v.data[3] = 2.0;
	v.data[4] = 1.0;

	count = zsl_vec_contains(&v, 0.0, 1E-5);
	zassert_equal(count, 1, NULL);
	count = zsl_vec_contains(&v, 1.0, 1E-5);
	zassert_equal(count, 2, NULL);
	count = zsl_vec_contains(&v, 2.0, 1E-5);
	zassert_equal(count, 2, NULL);
	count = zsl_vec_contains(&v, 3.0, 1E-5);
	zassert_equal(count, 0, NULL);
}

ZTEST(zsl_tests, test_vector_sort)
{
	int rc;

	ZSL_VECTOR_DEF(v, 5);
	ZSL_VECTOR_DEF(vs, 5);
	ZSL_VECTOR_DEF(vp, 5);
	ZSL_VECTOR_DEF(w, 5);
	ZSL_VECTOR_DEF(ws, 5);
	ZSL_VECTOR_DEF(wp, 5);

	/* Vector with no repeated values. */
	zsl_real_t a[5] = { 3.0, 4.5, -3.5, 0.0, -1.0 };
	zsl_real_t b[5] = { -3.5, -1.0, 0.0, 3.0, 4.5 };

	/* Vector with repeated values. */
	zsl_real_t c[5] = { -1.0, 4.5, 4.5, 0.0, -1.0 };
	zsl_real_t d[5] = { -1.0, -1.0, 0.0, 4.5, 4.5 };

	/* Assign arrays to vectors. */
	rc = zsl_vec_from_arr(&v, a);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&vs, b);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&w, c);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&ws, d);
	zassert_true(rc == 0, NULL);

	/* Perform the sorting operation. */
	rc = zsl_vec_sort(&v, &vp);
	zassert_true(rc == 0, NULL);
	zassert_equal(vp.data[0], vs.data[0], NULL);
	zassert_equal(vp.data[1], vs.data[1], NULL);
	zassert_equal(vp.data[2], vs.data[2], NULL);
	zassert_equal(vp.data[3], vs.data[3], NULL);
	zassert_equal(vp.data[4], vs.data[4], NULL);

	/* Test the function on an already sorted vector. */
	rc = zsl_vec_sort(&vs, &vp);
	zassert_true(rc == 0, NULL);
	zassert_equal(vp.data[0], vs.data[0], NULL);
	zassert_equal(vp.data[1], vs.data[1], NULL);
	zassert_equal(vp.data[2], vs.data[2], NULL);
	zassert_equal(vp.data[3], vs.data[3], NULL);
	zassert_equal(vp.data[4], vs.data[4], NULL);

	/* Perform the sorting operation. */
	rc = zsl_vec_sort(&w, &wp);
	zassert_true(rc == 0, NULL);
	zassert_equal(wp.data[0], ws.data[0], NULL);
	zassert_equal(wp.data[1], ws.data[1], NULL);
	zassert_equal(wp.data[2], ws.data[2], NULL);
	zassert_equal(wp.data[3], ws.data[3], NULL);
	zassert_equal(wp.data[4], ws.data[4], NULL);

	/* Test the function on an already sorted vector. */
	rc = zsl_vec_sort(&ws, &wp);
	zassert_true(rc == 0, NULL);
	zassert_equal(wp.data[0], ws.data[0], NULL);
	zassert_equal(wp.data[1], ws.data[1], NULL);
	zassert_equal(wp.data[2], ws.data[2], NULL);
	zassert_equal(wp.data[3], ws.data[3], NULL);
	zassert_equal(wp.data[4], ws.data[4], NULL);
}
