/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ztest.h>

extern void test_matrix_init(void);
extern void test_matrix_from_arr(void);
extern void test_matrix_get(void);
extern void test_matrix_set(void);

extern void test_vector_from_arr(void);
extern void test_vector_cross(void);
extern void test_vector_is_equal(void);

void test_main(void)
{
	ztest_test_suite(zsl_tests,
        ztest_unit_test(test_matrix_init),
        ztest_unit_test(test_matrix_from_arr),
        ztest_unit_test(test_matrix_get),
        ztest_unit_test(test_matrix_set),

        ztest_unit_test(test_vector_from_arr),
        ztest_unit_test(test_vector_cross),
        ztest_unit_test(test_vector_is_equal)
	);

	ztest_run_test_suite(zsl_tests);
}
