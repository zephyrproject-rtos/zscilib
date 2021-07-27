/*
 * Copyright (c) 2021 Marti Riba Pons
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr.h>
#include <sys/printk.h>
#include <zsl/zsl.h>
#include <zsl/orientation/orientation.h>
#include <zsl/orientation/fusion/fusion.h>
#include <zsl/orientation/fusion/madgwick.h>
#include <zsl/orientation/fusion/mahony.h>

static struct zsl_fus_drv madgwick_drv = {
	.init_handler = zsl_fus_madg_init,
	.feed_handler = zsl_fus_madg_feed,
	.error_handler = zsl_fus_madg_error,
};

static struct zsl_fus_drv mahony_drv = {
	.init_handler = zsl_fus_mahn_init,
	.feed_handler = zsl_fus_mahn_feed,
	.error_handler = zsl_fus_mahn_error,
};

void fusion_demo(struct zsl_fus_drv *drv)
{
	struct zsl_quat q = { 0 };
	struct zsl_euler e = { 0 };

	ZSL_VECTOR_DEF(a, 3);
	ZSL_VECTOR_DEF(m, 3);
	ZSL_VECTOR_DEF(g, 3);

	a.data[0] = 0.007823;
	a.data[1] = -1.012609;
	a.data[2] = -0.023313;

	m.data[0] = -66.306053;
	m.data[1] = -98.563057;
	m.data[2] = -43.526505;

	g.data[0] = 0.090443;
	g.data[1] = -0.282885;
	g.data[2] = -0.075588;

	/* Init filter at 100 Hz. */
	drv->init_handler(100);

	/* Feed the filter. */
	drv->feed_handler(&a, &m, &g, &q);

	/* Feed the filter with no mag data. */
	// drv->feed_handler(&a, NULL, &g, &q);

	zsl_quat_print(&q);
	zsl_quat_to_euler(&q, &e);
	e.x *= 180. / ZSL_PI;
	e.y *= 180. / ZSL_PI;
	e.z *= 180. / ZSL_PI;
	zsl_eul_print(&e);
}

void main(void)
{
	printf("Orientation API demo\n");
	printf("--------------------\n\n");

	printf("Madgwick:\n");
	fusion_demo(&madgwick_drv);

	printf("Mahony:\n");
	fusion_demo(&mahony_drv);
}
