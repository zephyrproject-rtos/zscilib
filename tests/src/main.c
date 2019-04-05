/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ztest.h>

extern void test_matrix_init(void);
extern void test_matrix_from_arr(void);
extern void test_matrix_copy(void);
extern void test_matrix_get(void);
extern void test_matrix_set(void);
extern void test_matrix_get_set_row(void);
extern void test_matrix_get_set_col(void);
extern void test_matrix_row_from_vec(void);
extern void test_matrix_unary_op(void);
extern void test_matrix_unary_func(void);
extern void test_matrix_binary_op(void);
extern void test_matrix_binary_func(void);
extern void test_matrix_add(void);
extern void test_matrix_add_d(void);
extern void test_matrix_sum_rows_d(void);
extern void test_matrix_sum_rows_scaled_d(void);
extern void test_matrix_sub(void);
extern void test_matrix_sub_d(void);
extern void test_matrix_mult_sq(void);
extern void test_matrix_mult_rect(void);
extern void test_matrix_scalar_mult_d(void);
extern void test_matrix_scalar_mult_row_d(void);
extern void test_matrix_trans(void);
extern void test_matrix_adj(void);
extern void test_matrix_deter(void);
extern void test_matrix_inv(void);
extern void test_matrix_inv_nxn(void);
extern void test_matrix_eigen(void);
extern void test_matrix_min(void);
extern void test_matrix_max(void);
extern void test_matrix_min_idx(void);
extern void test_matrix_max_idx(void);
extern void test_matrix_is_equal(void);
extern void test_matrix_is_notneg(void);

extern void test_vector_from_arr(void);
extern void test_vector_get_subset(void);
extern void test_vector_add(void);
extern void test_vector_sub(void);
extern void test_vector_neg(void);
extern void test_vector_sum(void);
extern void test_vector_magn(void);
extern void test_vector_scalar_add(void);
extern void test_vector_scalar_mult(void);
extern void test_vector_scalar_div(void);
extern void test_vector_dist(void);
extern void test_vector_dot(void);
extern void test_vector_norm(void);
extern void test_vector_to_unit(void);
extern void test_vector_cross(void);
extern void test_vector_sum_of_sqrs(void);
extern void test_vector_mean(void);
extern void test_vector_ar_mean(void);
extern void test_vector_rev(void);
extern void test_vector_is_equal(void);
extern void test_vector_is_notneg(void);

void test_main(void)
{
	ztest_test_suite(zsl_tests,
        ztest_unit_test(test_matrix_init),
        ztest_unit_test(test_matrix_from_arr),
        ztest_unit_test(test_matrix_copy),
        ztest_unit_test(test_matrix_get),
        ztest_unit_test(test_matrix_set),
        ztest_unit_test(test_matrix_get_set_row),
        ztest_unit_test(test_matrix_get_set_col),
        ztest_unit_test(test_matrix_row_from_vec),
        ztest_unit_test(test_matrix_unary_op),
        ztest_unit_test(test_matrix_unary_func),
        ztest_unit_test(test_matrix_binary_op),
        ztest_unit_test(test_matrix_binary_func),
        ztest_unit_test(test_matrix_add),
        ztest_unit_test(test_matrix_add_d),
        ztest_unit_test(test_matrix_sum_rows_d),
        ztest_unit_test(test_matrix_sum_rows_scaled_d),
        ztest_unit_test(test_matrix_sub),
        ztest_unit_test(test_matrix_sub_d),
        ztest_unit_test(test_matrix_mult_sq),
        ztest_unit_test(test_matrix_mult_rect),
        ztest_unit_test(test_matrix_scalar_mult_d),
        ztest_unit_test(test_matrix_scalar_mult_row_d),
        ztest_unit_test(test_matrix_trans),
        ztest_unit_test(test_matrix_adj),
        ztest_unit_test(test_matrix_deter),
        ztest_unit_test(test_matrix_inv),
        ztest_unit_test(test_matrix_inv_nxn),
        ztest_unit_test(test_matrix_eigen),
        ztest_unit_test(test_matrix_min),
        ztest_unit_test(test_matrix_max),
        ztest_unit_test(test_matrix_min_idx),
        ztest_unit_test(test_matrix_max_idx),
        ztest_unit_test(test_matrix_is_equal),
        ztest_unit_test(test_matrix_is_notneg),

        ztest_unit_test(test_vector_from_arr),
        ztest_unit_test(test_vector_get_subset),
        ztest_unit_test(test_vector_add),
        ztest_unit_test(test_vector_sub),
        ztest_unit_test(test_vector_neg),
        ztest_unit_test(test_vector_sum),
        ztest_unit_test(test_vector_magn),
        ztest_unit_test(test_vector_scalar_add),
        ztest_unit_test(test_vector_scalar_mult),
        ztest_unit_test(test_vector_scalar_div),
        ztest_unit_test(test_vector_dist),
        ztest_unit_test(test_vector_dot),
        ztest_unit_test(test_vector_norm),
        ztest_unit_test(test_vector_to_unit),
        ztest_unit_test(test_vector_cross),
        ztest_unit_test(test_vector_sum_of_sqrs),
        ztest_unit_test(test_vector_mean),
        ztest_unit_test(test_vector_ar_mean),
        ztest_unit_test(test_vector_rev),
        ztest_unit_test(test_vector_is_equal),
        ztest_unit_test(test_vector_is_notneg)
	);

	ztest_run_test_suite(zsl_tests);
}
