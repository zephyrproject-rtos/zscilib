/*
 * Copyright (c) 2019 Kevin Townsend
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zsl/colorimetry.h>

void main(void)
{
	printk("zscilib     : %s\n", zsl_version);
	printk("\nType 'help' for command options.\n");

	while (1) {
		k_sleep(K_FOREVER);
	}
}
