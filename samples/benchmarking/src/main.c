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
#include "timing_info.h"

char sline[256];
volatile u64_t vector_add_start_time;
volatile u64_t vector_add_end_time;

void print_settings(void)
{
    printk("BOARD:                       %s\n", CONFIG_BOARD);
    printk("ZSL VERSION:                 %s\n", ZSL_VERSION);
    printk("\n");

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
    u32_t loops = 100000;

    DECLARE_VAR(vector, add)

    ZSL_VECTOR_STATIC_INIT(va, 3);
    ZSL_VECTOR_STATIC_INIT(vb, 3);
    ZSL_VECTOR_STATIC_INIT(vc, 3);

    zsl_real_t a[3] = { 0.0, 1.0, 2.0 };
    zsl_real_t b[3] = {-1.0, 0.5, 0.1 };

    /* Initialise the vectors. */
    zsl_vec_init(&va);
    zsl_vec_init(&vb);
    zsl_vec_init(&vc);

    /* Assign arrays to vectors. */
    zsl_vec_from_arr(&va, a);
    zsl_vec_from_arr(&vb, b);

    /* Perform the add operation. */
    TIMING_INFO_PRE_READ();
    vector_add_start_time = TIMING_INFO_OS_GET_TIME();
    for (u32_t i = 0; i < loops; i++) {
        zsl_vec_add(&va, &vb, &vc);
    }
    TIMING_INFO_PRE_READ();
	vector_add_end_time = TIMING_INFO_OS_GET_TIME();

    u32_t time = (u32_t)((vector_add_end_time - vector_add_start_time) & 0xFFFFFFFFULL);
    printk("zsl_vec_add : %u ticks\n", time);
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
