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
#include <zsl/instrumentation.h>
#include "data.h"

static struct zsl_fus_madg_cfg madg_cfg = {
	.beta = 0.018,
};

static struct zsl_fus_drv madgwick_drv = {
	.init_handler = zsl_fus_madg_init,
	.feed_handler = zsl_fus_madg_feed,
	.error_handler = zsl_fus_madg_error,
	.config = &madg_cfg,
};

static zsl_real_t _mahn_intfb[3] = {0.0, 0.0, 0.0};
static struct zsl_fus_mahn_cfg mahn_cfg = {
	.kp = 0.028,
	.ki = 0.02,
	.intfb = {
		.sz = 3,
		.data = _mahn_intfb,
	},
};

static struct zsl_fus_drv mahony_drv = {
	.init_handler = zsl_fus_mahn_init,
	.feed_handler = zsl_fus_mahn_feed,
	.error_handler = zsl_fus_mahn_error,
	.config = &mahn_cfg,
};

static struct zsl_fus_drv saam_drv = {
	.init_handler = zsl_fus_saam_init,
	.feed_handler = zsl_fus_saam_feed,
	.error_handler = zsl_fus_saam_error,
};

static struct zsl_fus_aqua_cfg aqua_cfg = {
	.alpha = 0.0,
	.beta = 0.0,
	.e_a = 0.0,
	.e_m = 0.0,
};

static struct zsl_fus_drv aqua_drv = {
	.init_handler = zsl_fus_aqua_init,
	.feed_handler = zsl_fus_aqua_feed,
	.error_handler = zsl_fus_aqua_error,
	.config = &aqua_cfg,
};

static struct zsl_fus_comp_cfg comp_cfg = {
	.alpha = 0.0001,
};

static struct zsl_fus_drv comp_drv = {
	.init_handler = zsl_fus_comp_init,
	.feed_handler = zsl_fus_comp_feed,
	.error_handler = zsl_fus_comp_error,
	.config = &comp_cfg,
};

static struct zsl_fus_kalm_cfg kalm_cfg = {
	.var_g = 0.01,
};

static struct zsl_fus_drv kalm_drv = {
	.init_handler = zsl_fus_kalm_init,
	.feed_handler = zsl_fus_kalm_feed,
	.error_handler = zsl_fus_kalm_error,
	.config = &kalm_cfg,
};


void fusion_demo(struct zsl_fus_drv *drv)
{
	struct zsl_quat q = { .r = 1.0, .i = 0.0, .j = 0.0, .k = 0.0 };
	struct zsl_euler e = { 0 };
	// uint32_t ns = 0;

	ZSL_VECTOR_DEF(av, 3);
	ZSL_VECTOR_DEF(mv, 3);
	ZSL_VECTOR_DEF(gv, 3);

	// ZSL_VECTOR_DEF(a, 3);
	// ZSL_VECTOR_DEF(m, 3);
	// ZSL_VECTOR_DEF(g, 3);

	// a.data[0] = 0.01;
	// a.data[1] = -1.01;
	// a.data[2] = -0.02;

	// m.data[0] = -66.0;
	// m.data[1] = -98.0;
	// m.data[2] = -43.0;

	// g.data[0] = 0.09;
	// g.data[1] = -0.28;
	// g.data[2] = -0.07;

	/* Init filter at 100 Hz. */
	drv->init_handler(100.0, drv->config);

	for (size_t i = 0; i < 201; i++) {
		zsl_mtx_get_row(&zsl_fus_data_acc, i, av.data);
		zsl_mtx_get_row(&zsl_fus_data_mag, i, mv.data);
		zsl_mtx_get_row(&zsl_fus_data_gyr, i, gv.data);
		drv->feed_handler(&av, &mv, &gv, &q, drv->config);
	}



	// /* Feed the filter. */
	// ZSL_INSTR_START(ns);
	// drv->feed_handler(&a, NULL, &g, &q, drv->config);
	// ZSL_INSTR_STOP(ns);
	// printf("Took: %d ns\n", ns);
	// /* Feed the filter with no mag data. */
	// drv->feed_handler(&a, NULL, &g, &q, drv->config);

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

	printf("SAAM:\n");
	fusion_demo(&saam_drv);

	printf("AQUA:\n");
	fusion_demo(&aqua_drv);

	printf("Complementary:\n");
	fusion_demo(&comp_drv);

	printf("Kalman:\n");
	fusion_demo(&kalm_drv);
}


