/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <misc/printk.h>
#include <zsl/zsl.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME 	1000

void main(void)
{
    printk("zscilib benchmark (board: %s)\n", CONFIG_BOARD);

	while (1) {
		k_sleep(SLEEP_TIME);
	}
}
