/*
 * Copyright (c) 2019 Kevin Townsend
 *
 * SPDX-License-Identifier: Apache-2.0
 */


#include <zephyr.h>
#include <misc/printk.h>
#include <zsl/zsl.h>
#include <zsl/matrices.h>

void test_mtx_mult_demo(void)
{
        int rc;
        zsl_real_t ma_data[12] = {
            1.0, 2.0, 3.0,
            4.0, 5.0, 6.0,
            7.0, 8.0, 9.0,
            1.0, 1.0, 1.0
        };

        zsl_real_t mb_data[12] = {
            1.0, 2.0, 3.0, 4.0,
            5.0, 6.0, 7.0, 8.0,
            1.0, 1.0, 1.0, 1.0
        };

        ZSL_MATRIX_DEF(ma, 4, 3);   /* 4x3 matrix */
        ZSL_MATRIX_DEF(mb, 3, 4);   /* 3x4 matrix */
        ZSL_MATRIX_DEF(mc, 4, 4);   /* Resulting 4x4 matrix. */

        /* Assign data to input matrices. */
        ma.data = ma_data;
        mb.data = mb_data;

        /* Initialise output matrix. */
        rc = zsl_mtx_init(&mc, NULL);

        rc = zsl_mtx_mult(&ma, &mb, &mc);
        if (rc) {
            printk("Error performing matrix multuplication: %d\n", rc);
            return;
        }

        printk("mtx multuply output:\n");
        zsl_mtx_print(&mc);
}

void main(void)
{
        printk("zscilib matrix mult demo\n\n");

        test_mtx_mult_demo();

        while (1) {
                k_sleep(100);
        }
}
