/*
 * Copyright (c) 2019 Kevin Townsend
 *
 * SPDX-License-Identifier: Apache-2.0
 */


#include <zephyr.h>
#include <misc/printk.h>
#include <zsl/zsl.h>
#include <zsl/matrices.h>

void test_svd_demo(void)
{
        int rc;
        ZSL_MATRIX_DEF(m, 3, 3);

        /* Initialise the matrix with the default (empty) entry_fn. */
        rc = zsl_mtx_init(&m, NULL);

        printk("SVD output = TODO (%u ns)\n", 0);
}

void main(void)
{
        printk("zscilib svd demo\n\n");

        test_svd_demo();

        while (1) {
                k_sleep(100);
        }
}
