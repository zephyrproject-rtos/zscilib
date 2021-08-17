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
	.beta = 0.174,
};

static struct zsl_fus_drv madgwick_drv = {
	.init_handler = zsl_fus_madg_init,
	.feed_handler = zsl_fus_madg_feed,
	.error_handler = zsl_fus_madg_error,
	.config = &madg_cfg,
};

static zsl_real_t _mahn_intfb[3] = {0.0, 0.0, 0.0};
static struct zsl_fus_mahn_cfg mahn_cfg = {
	.kp = 0.235,
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
	.alpha = 0.7,
	.beta = 0.7,
	.e_a = 0.9,
	.e_m = 0.9,
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

static zsl_real_t _kalm_P[16] = { 
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0
};

static struct zsl_fus_kalm_cfg kalm_cfg = {
	.var_g = 0.3 * 0.3,
	.var_a = 0.5 * 0.5,
	.var_m = 0.8 * 0.8,
	.P = {
		.sz_rows = 4,
		.sz_cols = 4,
		.data = _kalm_P,
	},
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
	uint32_t ns = 0;

	// ZSL_VECTOR_DEF(av, 3);
	// ZSL_VECTOR_DEF(mv, 3);
	// ZSL_VECTOR_DEF(gv, 3);

	ZSL_VECTOR_DEF(a, 3);
	ZSL_VECTOR_DEF(m, 3);
	ZSL_VECTOR_DEF(g, 3);

	a.data[0] = 0.01;
	a.data[1] = -1.01;
	a.data[2] = -0.02;

	m.data[0] = -66.0;
	m.data[1] = -98.0;
	m.data[2] = -43.0;

	g.data[0] = 0.09;
	g.data[1] = -0.28;
	g.data[2] = -0.07;

	/* Init filter at 100 Hz. */
	drv->init_handler(100.0, drv->config);
	zsl_real_t dip = 4.0 * ZSL_PI / (3.0 * 180.0);

	// for (size_t i = 0; i < 200; i++) {
	// 	zsl_mtx_get_row(&zsl_fus_data_acc, i, av.data);
	// 	zsl_mtx_get_row(&zsl_fus_data_mag, i, mv.data);
	// 	zsl_mtx_get_row(&zsl_fus_data_gyr, i, gv.data);
	// 	drv->feed_handler(&av, &mv, &gv, NULL, &q, drv->config);
	// }

	/* Feed the filter. */
	ZSL_INSTR_START(ns);
	drv->feed_handler(NULL, &m, &g, &dip, &q, drv->config);
	ZSL_INSTR_STOP(ns);
	printf("Took: %d ns\n", ns);
	// /* Feed the filter with no mag data. */
	// drv->feed_handler(&a, NULL, &g, &q, drv->config);

	zsl_quat_print(&q);
	zsl_quat_to_euler(&q, &e);
	e.x *= 180. / ZSL_PI;
	e.y *= 180. / ZSL_PI;
	e.z *= 180. / ZSL_PI;
	zsl_eul_print(&e);
}

void cal_demo(void)
{	
	// zsl_real_t l = 1.0;
	// zsl_real_t mu = 10.0;
	// ZSL_MATRIX_DEF(K, 3, 3);
	// ZSL_VECTOR_DEF(b, 3);
	// zsl_fus_cal_magn(&zsl_fus_data_magn_cal, &l, &mu, &K, &b);
	// zsl_mtx_print(&K);
	// zsl_vec_print(&b);

	// zsl_real_t data[9] = { 0.989337, 0.033727, -0.005689,
	// 					   0.033727, 1.019691, -0.006163,
	// 					  -0.005689, -0.006163, 0.964197 };
	// zsl_mtx_from_arr(&K, data);
	// b.data[0] = 10.782474;
	// b.data[1] = -5.596682;
	// b.data[2] = 23.938386;

	// ZSL_MATRIX_DEF(mout, zsl_fus_data_magn_cal.sz_rows, zsl_fus_data_magn_cal.sz_cols);
	// zsl_fus_cal_magn_corr(&zsl_fus_data_magn_cal, &K, &b, &mout);
	// ZSL_VECTOR_DEF(H, 3);
	// for (size_t i = 0; i < zsl_fus_data_magn_cal.sz_rows; i++) {
	// 	zsl_mtx_get_row(&mout, i, H.data);
	// 	printf ("(%lf, %lf, %lf), ", H.data[0], H.data[1], H.data[2]);
	// }
}

void madg_cal_demo(void)
{	
	zsl_real_t beta;
	zsl_real_t dip = 4.0 * ZSL_PI / (3.0 * 180.0);
	zsl_fus_cal_madg(&zsl_fus_data_gyr, &zsl_fus_data_acc, &zsl_fus_data_mag,
				100.0, &dip, &beta);
	printf("Beta is %f\n\n", beta);
}

void mahn_cal_demo(void)
{	
	zsl_real_t kp;
	zsl_real_t dip = 4.0 * ZSL_PI / (3.0 * 180.0);
	zsl_fus_cal_mahn(&zsl_fus_data_gyr, &zsl_fus_data_acc, &zsl_fus_data_mag,
				100.0, &dip, &kp);
	printf("Kp is %f\n\n", kp);
}


void main(void)
{
	printf("Orientation API demo\n");
	printf("--------------------\n\n");

	// printf("Madgwick:\n");
	// fusion_demo(&madgwick_drv);

	// printf("Mahony:\n");
	// fusion_demo(&mahony_drv);

	// printf("SAAM:\n");
	// fusion_demo(&saam_drv);

	// printf("AQUA:\n");
	// fusion_demo(&aqua_drv);

	// printf("Complementary:\n");
	// fusion_demo(&comp_drv);

	// printf("Kalman:\n");
	// fusion_demo(&kalm_drv);

	// cal_demo();

	// madg_cal_demo();

	// mahn_cal_demo();

	int rc = 0;

	ZSL_MATRIX_DEF(K, 3, 3);
	ZSL_VECTOR_DEF(b, 3);
	ZSL_VECTOR_DEF(h, 3);
	ZSL_VECTOR_DEF(h_out, 3);

	zsl_real_t a[9] = {
		0.99, 0.12, 0.01,
		0.12, 0.87, 0.09,
		0.01, 0.09, 0.92
	};

	zsl_real_t c[3] = { 1.2, -4.9, 5.7 };
	zsl_real_t d[3] = { 0.8, 1.2, 5.4 };

	/* Assign arrays to vectors and matrices. */
	rc = zsl_mtx_from_arr(&K, a);
	rc = zsl_vec_from_arr(&b, c);
	rc = zsl_vec_from_arr(&h, d);

	/* Correct the magnetometer data. */
	rc = zsl_fus_cal_magn_corr(&h, &K, &b, &h_out);



	zsl_vec_print(&h_out);
}


