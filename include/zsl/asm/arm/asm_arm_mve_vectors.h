/*
 * Copyright (c) 2026 Richard Wheatley
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Optimised vector functions for zscilib using Armv8.1-M MVE (Helium).
 *
 * This file contains vector functions optimised with MVE intrinsics for
 * Armv8.1-M cores (Cortex-M55, Cortex-M85). MVE-FP only supports f16/f32,
 * so these implementations require CONFIG_ZSL_SINGLE_PRECISION.
 *
 * Loop tails are handled with predication (VCTP), so no scalar epilogue is
 * required. Toolchains supporting tail-predicated low-overhead loops
 * (DLSTP/LETP) will collapse these loops automatically.
 */

#include <zsl/zsl.h>

#ifndef ZEPHYR_INCLUDE_ZSL_ASM_ARM_MVE_VECTORS_H_
#define ZEPHYR_INCLUDE_ZSL_ASM_ARM_MVE_VECTORS_H_

#if CONFIG_ZSL_PLATFORM_OPT == 3

#if !defined(__ARM_FEATURE_MVE) || !(__ARM_FEATURE_MVE & 2)
#error "CONFIG_ZSL_PLATFORM_OPT=3 requires MVE-FP (e.g. -mcpu=cortex-m55)."
#endif

#ifndef CONFIG_ZSL_SINGLE_PRECISION
#error "MVE has no f64 datapath. Level 3 needs CONFIG_ZSL_SINGLE_PRECISION."
#endif

#include <arm_mve.h>

#if !asm_vec_add
int zsl_vec_add(struct zsl_vec *v, struct zsl_vec *w, struct zsl_vec *x)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure v and w are equal length. */
	if ((v->sz != w->sz) || (v->sz != x->sz)) {
		return -EINVAL;
	}
#endif

	float32_t *pv = v->data;
	float32_t *pw = w->data;
	float32_t *px = x->data;
	int32_t cnt = (int32_t)v->sz;

	while (cnt > 0) {
		mve_pred16_t p = vctp32q((uint32_t)cnt);
		float32x4_t va = vldrwq_z_f32(pv, p);
		float32x4_t vb = vldrwq_z_f32(pw, p);

		vstrwq_p_f32(px, vaddq_x_f32(va, vb, p), p);
		pv += 4;
		pw += 4;
		px += 4;
		cnt -= 4;
	}

	return 0;
}
#define asm_vec_add 1
#endif

#if !asm_vec_scalar_mult
int zsl_vec_scalar_mult(struct zsl_vec *v, zsl_real_t s)
{
	float32_t *pv = v->data;
	int32_t cnt = (int32_t)v->sz;

	while (cnt > 0) {
		mve_pred16_t p = vctp32q((uint32_t)cnt);
		float32x4_t va = vldrwq_z_f32(pv, p);

		vstrwq_p_f32(pv, vmulq_x_n_f32(va, s, p), p);
		pv += 4;
		cnt -= 4;
	}

	return 0;
}
#define asm_vec_scalar_mult 1
#endif

#if !asm_vec_dot
int zsl_vec_dot(struct zsl_vec *v, struct zsl_vec *w, zsl_real_t *d)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure v and w are equal length. */
	if (v->sz != w->sz) {
		return -EINVAL;
	}
#endif

	float32x4_t acc = vdupq_n_f32(0.0f);
	float32_t *pv = v->data;
	float32_t *pw = w->data;
	int32_t cnt = (int32_t)v->sz;

	while (cnt > 0) {
		mve_pred16_t p = vctp32q((uint32_t)cnt);
		float32x4_t va = vldrwq_z_f32(pv, p);
		float32x4_t vb = vldrwq_z_f32(pw, p);

		acc = vfmaq_m_f32(acc, va, vb, p);
		pv += 4;
		pw += 4;
		cnt -= 4;
	}

	*d = vgetq_lane_f32(acc, 0) + vgetq_lane_f32(acc, 1) +
	     vgetq_lane_f32(acc, 2) + vgetq_lane_f32(acc, 3);

	return 0;
}
#define asm_vec_dot 1
#endif

#if !asm_vec_sum_of_sqrs
zsl_real_t zsl_vec_sum_of_sqrs(struct zsl_vec *v)
{
	float32x4_t acc = vdupq_n_f32(0.0f);
	float32_t *pv = v->data;
	int32_t cnt = (int32_t)v->sz;

	while (cnt > 0) {
		mve_pred16_t p = vctp32q((uint32_t)cnt);
		float32x4_t va = vldrwq_z_f32(pv, p);

		acc = vfmaq_m_f32(acc, va, va, p);
		pv += 4;
		cnt -= 4;
	}

	return vgetq_lane_f32(acc, 0) + vgetq_lane_f32(acc, 1) +
	       vgetq_lane_f32(acc, 2) + vgetq_lane_f32(acc, 3);
}
#define asm_vec_sum_of_sqrs 1
#endif

#endif /* CONFIG_ZSL_PLATFORM_OPT == 3 */
#endif /* ZEPHYR_INCLUDE_ZSL_ASM_ARM_MVE_VECTORS_H_ */
