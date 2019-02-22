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
    u64_t vector_add_start_time;
    u64_t vector_add_end_time;

    ZSL_VECTOR_DEF(va, 3);
    ZSL_VECTOR_DEF(vb, 3);
    ZSL_VECTOR_DEF(vc, 3);

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

    printk("zsl_vec_add cycles: %u\n",
    		(u32_t)((vector_add_end_time - vector_add_start_time) &
    			   0xFFFFFFFFULL));
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
