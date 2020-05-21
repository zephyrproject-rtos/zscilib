/*
 * Copyright (c) 2019-2020 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Optimised vector functions for zscilib using ARM Thumb or ARM Thumb2.
 *
 * This file contains optimised vector functions for ARM Thumb or ARM Thumb2.
 */

#include <zsl/zsl.h>
#include <zsl/asm/arm/asm_arm.h>

#ifndef ZEPHYR_INCLUDE_ZSL_ASM_ARM_VECTORS_H_
#define ZEPHYR_INCLUDE_ZSL_ASM_ARM_VECTORS_H_

#if !asm_vec_add
#if CONFIG_ZSL_PLATFORM_OPT == 2
int zsl_vec_add(struct zsl_vec *v, struct zsl_vec *w, struct zsl_vec *x)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure v and w are equal length. */
	if ((v->sz != w->sz) || (v->sz != x->sz)) {
		return -EINVAL;
	}
#endif

	for (size_t i = 0; i < v->sz; i++) {
		x->data[i] = v->data[i] + w->data[i];
	}

	return 0;
}
#define asm_vec_add 1
#endif
#endif

#if !asm_vec_scalar_add
#if CONFIG_ZSL_PLATFORM_OPT == 2
/* TODO: ARM Thumb2 GNU implementation. */
int zsl_vec_scalar_add(struct zsl_vec *v, zsl_real_t s)
{
	for (size_t i = 0; i < v->sz; i++) {
		v->data[i] += s;
	}

	return 0;
}
#define asm_vec_scalar_add 1
#endif
#endif

#if !asm_vec_scalar_mult
#if CONFIG_ZSL_PLATFORM_OPT == 2
/* TODO: ARM Thumb2 GNU implementation. */
int zsl_vec_scalar_mult(struct zsl_vec *v, zsl_real_t s)
{
	for (size_t i = 0; i < v->sz; i++) {
		v->data[i] *= s;
	}

	return 0;
}
#define asm_vec_scalar_mult 1
#endif /* CONFIG_ZSL_PLATFORM_OPT == 2 */
#endif

#if !asm_vec_scalar_div
#if CONFIG_ZSL_PLATFORM_OPT == 2
/* TODO: ARM Thumb2 GNU implementation. */
int zsl_vec_scalar_div(struct zsl_vec *v, zsl_real_t s)
{
	/* Avoid divide by zero errors. */
	if (s == 0) {
		return -EINVAL;
	}

	for (size_t i = 0; i < v->sz; i++) {
		v->data[i] /= s;
	}

	return 0;
}
#define asm_vec_scalar_div 1
#endif /* CONFIG_ZSL_PLATFORM_OPT == 2 */
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_ASM_ARM_VECTORS_H_ */
