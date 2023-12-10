/*
 * Copyright (c) 2021 Marti Riba Pons
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zsl/zsl.h>
#include <zsl/orientation/orientation.h>
#include <zsl/instrumentation.h>
#include "data.h"

/* Config settings for the Madgwick filter. */
static struct zsl_fus_madg_cfg madg_cfg = {
	.beta = 0.174,
};

static struct zsl_fus_drv madgwick_drv = {
	.init_handler = zsl_fus_madg_init,
	.feed_handler = zsl_fus_madg_feed,
	.error_handler = zsl_fus_madg_error,
	.config = &madg_cfg,
};

/* Config settings for the Mahoney filter. */
static zsl_real_t _mahn_intfb[3] = { 0.0, 0.0, 0.0 };
static struct zsl_fus_mahn_cfg mahn_cfg = {
	.kp = 0.235,
	.ki = 0.02,
	.integral_limit = 10000.0,
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

/* Settings for the SAAM filter. */
static struct zsl_fus_drv saam_drv = {
	.init_handler = zsl_fus_saam_init,
	.feed_handler = zsl_fus_saam_feed,
	.error_handler = zsl_fus_saam_error,
};

/* Config settings for the AQUA filter. */
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

/* Config settings for the complementary filter. */
static struct zsl_fus_comp_cfg comp_cfg = {
	.alpha = 0.0001,
};

static struct zsl_fus_drv comp_drv = {
	.init_handler = zsl_fus_comp_init,
	.feed_handler = zsl_fus_comp_feed,
	.error_handler = zsl_fus_comp_error,
	.config = &comp_cfg,
};

/* Config settings for the extended Kalman filter. */
static zsl_real_t _kalm_P[16] = {
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0
};

static struct zsl_fus_kalm_cfg kalm_cfg = {
	.var_g = 0.001,
	.var_a = 0.307,
	.var_m = 0.2,
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

	ZSL_VECTOR_DEF(av, 3);
	ZSL_VECTOR_DEF(mv, 3);
	ZSL_VECTOR_DEF(gv, 3);

	/* Init filter at 100 Hz. */
	drv->init_handler(100.0, drv->config);

	/* Calculate magnetic inclination for given date and location. */
	/* Dubai @ 31-Aug-2021 = 39d 55' 24" D decl. in dms (D = positive). */
	// zsl_real_t incl;
	// zsl_comp_dms_to_dd(39, 55, 24, &incl);

	/* Feed all the samples into the filter engine. */
	for (size_t i = 0; i < DATA_SAMPLES; i++) {
		zsl_mtx_get_row(&zsl_fus_data_acc, i, av.data);
		zsl_mtx_get_row(&zsl_fus_data_mag, i, mv.data);
		zsl_mtx_get_row(&zsl_fus_data_gyr, i, gv.data);
		drv->feed_handler(&av, &mv, &gv, NULL, &q, drv->config);
	}

	/* Feed the filter the final sample. */
	zsl_mtx_get_row(&zsl_fus_data_acc, DATA_SAMPLES, av.data);
	zsl_mtx_get_row(&zsl_fus_data_mag, DATA_SAMPLES, mv.data);
	zsl_mtx_get_row(&zsl_fus_data_gyr, DATA_SAMPLES, gv.data);
	ZSL_INSTR_START(ns);
	drv->feed_handler(&av, &mv, &gv, NULL, &q, drv->config);
	ZSL_INSTR_STOP(ns);
	printf("Took: %d ns\n", ns);

	zsl_quat_print(&q);
	zsl_quat_to_euler(&q, &e);
	e.x *= 180. / ZSL_PI;
	e.y *= 180. / ZSL_PI;
	e.z *= 180. / ZSL_PI;
	zsl_eul_print(&e);
}

void madg_cal_demo(void)
{
	zsl_real_t beta;        /* Beta parameter in Madgwick algorithm. */

	zsl_fus_cal_madg(&zsl_fus_data_gyr, &zsl_fus_data_acc,
			 &zsl_fus_data_mag, 100.0, NULL, &beta);
	printf("Beta is %f\n\n", beta);

	/* Change the value of 'beta' in the filter to the calculated one. */
	madg_cfg.beta = beta;
}

void mahn_cal_demo(void)
{
	zsl_real_t kp;  /* kp parameter in Mahony algorithm. */

	zsl_fus_cal_mahn(&zsl_fus_data_gyr, &zsl_fus_data_acc,
			 &zsl_fus_data_mag, 100.0, NULL, &kp);
	printf("Kp is %f\n\n", kp);

	/* Change the value of 'kp' in the filter to the calculated one. */
	mahn_cfg.kp = kp;
}

void magn_cal_demo(void)
{
	/* Earth's magnetic field module in micro Testa as of August 28th 2021
	 * in Dubai, UAE. */
	zsl_real_t me = 44.3867;        /* Earth's magnetic field module. */

	ZSL_MATRIX_DEF(K, 3, 3);        /* Soft-iron corr. matrix. */
	ZSL_VECTOR_DEF(b, 3);           /* Hard-iron offset corr. vector. */

	/* Calculate the correction matrix and vector. */
	zsl_fus_cal_magn_fast(&zsl_fus_data_magn_cal, &me, &K, &b);
	printf("Soft-iron correction matrix:\n");
	zsl_mtx_print(&K);
	printf("Hard-iron offset correction vector:\n");
	zsl_vec_print(&b);

	ZSL_MATRIX_DEF(m_out, zsl_fus_data_magn_cal.sz_rows, 3);
	ZSL_VECTOR_DEF(H, 3);
	ZSL_VECTOR_DEF(H_out, 3);
	for (size_t i = 0; i < zsl_fus_data_magn_cal.sz_rows; i++) {
		zsl_mtx_get_row(&zsl_fus_data_magn_cal, i, H.data);
		zsl_fus_cal_corr_vec(&H, &K, &b, &H_out);
		zsl_mtx_set_row(&m_out, i, H_out.data);
	}
}

void compass_demo(void)
{
	zsl_real_t mn = 0.0;
	zsl_real_t gn = 0.0;
	zsl_real_t decl = 0.0;

	ZSL_VECTOR_DEF(m, 3);

	m.data[0] = 12.190000;
	m.data[1] = 27.620000;
	m.data[2] = -64.060000;

	printf("Input: ");
	zsl_vec_print(&m);

	/* Calculate magnetic north for reference sake. */
	zsl_comp_magn_north(&m, &mn);
	printf("Magnetic North       = %.6f\n", mn);

	/* Calculate magnetic declination for given date and location. */
	/* Dubai @ 28-Aug-2021 = 2d 13' E declination in dms (E = positive). */
	zsl_comp_dms_to_dd(2, 13, 0, &decl);
	printf("Magnetic declination = %.6f\n", decl);

	/* Calculate true north, applying magnetic declination. */
	zsl_comp_geo_north(&m, decl, &gn);
	printf("Geographic North     = %.6f\n", gn);
}

void main(void)
{
	printf("Orientation API demo\n");
	printf("--------------------\n\n");

	printf("Some of the calibration functions take several seconds\n");
	printf("to complete. Please be patient.\n\n");

	printf("Madgwick parameter calculation:\n");
	madg_cal_demo();

	printf("Madgwick:\n");
	fusion_demo(&madgwick_drv);

	printf("Mahoney parameter calculation:\n");
	mahn_cal_demo();

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

	printf("Magnetometer calibration:\n");
	magn_cal_demo();

	printf("Compass Test:\n");
	compass_demo();
}
