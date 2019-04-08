/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */


#include <zephyr.h>
#include <misc/printk.h>
#include <zsl/zsl.h>
#include <zsl/vectors.h>
#include <tc_util.h>

/** The number of times to execute the code under test. */
#define BENCH_LOOPS (10000U)

/* High-precision kernel clock. */
u32_t start_time;
u32_t stop_time;
u32_t cycles_spent;
u32_t nanoseconds_spent;

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
        zsl_real_t b[3] = {-1.0, 0.5, 0.1 };

        /* Assign arrays to vectors. */
        zsl_vec_from_arr(&va, a);
        zsl_vec_from_arr(&vb, b);

        /* Perform the add operation, tracking start and end times. */
        start_time = k_cycle_get_32();
        for (u32_t i = 0; i < BENCH_LOOPS; i++) {
                zsl_vec_add(&va, &vb, &vc);
        }
        stop_time = k_cycle_get_32();

        /* compute how long the work took (assumes no counter rollover) */
        cycles_spent = stop_time - start_time;
        nanoseconds_spent = SYS_CLOCK_HW_CYCLES_TO_NS(cycles_spent);

        printk("zsl_vec_add : %u ns\n", nanoseconds_spent / BENCH_LOOPS);
}

void main(void)
{
        printk("zscilib benchmark\n\n");
        print_settings();

        test_vec_add();

        while (1) {
                k_sleep(100);
        }
}
