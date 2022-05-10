/*
 * Copyright (c) 2021 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zsl/zsl.h>
#include <zsl/vectors.h>
#include <zsl/instrumentation.h>

/** The number of times to execute the code under test. */
#define BENCH_LOOPS (10000U)

void print_settings(void)
{
	printk("BOARD:                       %s\n", CONFIG_BOARD);
	printk("ZSL VERSION:                 %s (%s)\n", ZSL_VERSION,
	       ZSL_VERSION_DATE);

	printk("CONFIG_ZSL_PLATFORM_OPT:     %i\n", CONFIG_ZSL_PLATFORM_OPT);
#if CONFIG_ZSL_SINGLE_PRECISION
	printk("CONFIG_ZSL_SINGLE_PRECISION: True\n");
#else
	printk("CONFIG_ZSL_SINGLE_PRECISION: False\n");
#endif
#if CONFIG_ZSL_VECTOR_INLINE
	printk("CONFIG_ZSL_VECTOR_INLINE:    True\n");
#else
	printk("CONFIG_ZSL_VECTOR_INLINE:    False\n");
#endif
#if CONFIG_ZSL_MATRIX_INLINE
	printk("CONFIG_ZSL_MATRIX_INLINE:    True\n");
#else
	printk("CONFIG_ZSL_MATRIX_INLINE:    False\n");
#endif
#if CONFIG_ZSL_BOUNDS_CHECKS
	printk("CONFIG_ZSL_BOUNDS_CHECKS:    True\n");
#else
	printk("CONFIG_ZSL_BOUNDS_CHECKS:    False\n");
#endif
	printk("\n");
}

void test_vec_add(void)
{
	uint32_t instr;
	uint32_t instr_total = 0;

	ZSL_VECTOR_DEF(va, 3);
	ZSL_VECTOR_DEF(vb, 3);
	ZSL_VECTOR_DEF(vc, 3);

	zsl_real_t a[3] = { 0.0, 1.0, 2.0 };
	zsl_real_t b[3] = { -1.0, 0.5, 0.1 };

	/* Assign arrays to vectors. */
	zsl_vec_from_arr(&va, a);
	zsl_vec_from_arr(&vb, b);

	/* Init vc with zero values. */
	zsl_vec_init(&vc);

	ZSL_INSTR_START(instr);
	for (uint32_t i = 0; i < BENCH_LOOPS; i++) {
		zsl_vec_add(&va, &vb, &vc);
	}
	ZSL_INSTR_STOP(instr);
	instr_total += instr;

	printk("zsl_vec_add (avg): %u ns\n", instr_total / BENCH_LOOPS);
}

void main(void)
{
	printk("zscilib benchmark\n\n");
	print_settings();

	while (1) {
		test_vec_add();
		k_sleep(K_FOREVER);
	}
}
