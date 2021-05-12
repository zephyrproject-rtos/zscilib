/*
 * Copyright (c) 2019-2020 Kevin Townsend (KTOWN)
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <sys/printk.h>
#include <zsl/zsl.h>
#include <zsl/vectors.h>

#if !CONFIG_BOARD_QEMU_CORTEX_M3 && \
	(CONFIG_CPU_CORTEX_M3 || CONFIG_CPU_CORTEX_M4)
/* Use DWT cycle counter for precision timing if this is a HW M3/M4 core. */
u32_t dwt_count;
#define DWT_RESET_CYCLECOUNTER    do {				  \
		CoreDebug->DEMCR = CoreDebug->DEMCR | 0x01000000; \
		DWT->CYCCNT = 0;				  \
		DWT->CTRL = DWT->CTRL | 1; } while(0)
#else
/* Use the high-precision kernel clock. */
u32_t start_time;
u32_t stop_time;
u32_t cycles_spent;
u32_t nanoseconds_spent;
#endif

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

#if !CONFIG_BOARD_QEMU_CORTEX_M3 && \
	(CONFIG_CPU_CORTEX_M3 || CONFIG_CPU_CORTEX_M4)
	DWT_RESET_CYCLECOUNTER;
#else
	/* Perform the add operation, tracking start and end times. */
	start_time = k_cycle_get_32();
#endif

	for (u32_t i = 0; i < BENCH_LOOPS; i++) {
		zsl_vec_add(&va, &vb, &vc);
	}

#if !CONFIG_BOARD_QEMU_CORTEX_M3 && \
	(CONFIG_CPU_CORTEX_M3 || CONFIG_CPU_CORTEX_M4)
	dwt_count = DWT->CYCCNT;
#else
	stop_time = k_cycle_get_32();
	/* compute how long the work took (assumes no counter rollover) */
	cycles_spent = stop_time - start_time;
	nanoseconds_spent = (u32_t)k_cyc_to_ns_floor64(cycles_spent);
#endif

#if !CONFIG_BOARD_QEMU_CORTEX_M3 && \
	(CONFIG_CPU_CORTEX_M3 || CONFIG_CPU_CORTEX_M4)
	printk("zsl_vec_add : %u cycles\n", dwt_count / BENCH_LOOPS);
#else
	printk("zsl_vec_add : %u ns\n", nanoseconds_spent / BENCH_LOOPS);
#endif
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
