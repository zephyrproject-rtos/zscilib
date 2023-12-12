/*
 * Copyright (c) 2019-2021 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief API header file for zscilib.
 *
 * This file contains the entry points to the zscilib APIs.
 */

#ifndef ZEPHYR_INCLUDE_ZSL_H_
#define ZEPHYR_INCLUDE_ZSL_H_

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <zsl/consts.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Version numbers take the form of "MAJOR.MINOR.REVISION-STATUS".
 *
 * ZSL_VERSION_STATUS should be one of:
 *
 *   o "-prerelease"
 *   o "-alpha"
 *   o "-beta"
 *   o "-rc0", "-rc1", etc.
 *   o "-release"
 */

#define ZSL_VERSION             "0.2.0-rc1"
#define ZSL_VERSION_MAJOR       0
#define ZSL_VERSION_MINOR       2
#define ZSL_VERSION_REVISION    0
#define ZSL_VERSION_STATUS      "-rc1"
#define ZSL_VERSION_DATE        __DATE__

extern const char *zsl_version;

#if CONFIG_ZSL_SINGLE_PRECISION
typedef float zsl_real_t;
#else
typedef double zsl_real_t;
#endif

/* Map math functions based on single or double precision. */
/* TODO: Add an option for fast approximations of trig operations. */
#if CONFIG_ZSL_SINGLE_PRECISION
#define ZSL_CEIL       ceilf
#define ZSL_FLOOR      floorf
#define ZSL_ROUND      roundf
#define ZSL_ABS        fabsf
#define ZSL_MIN        fminf
#define ZSL_MAX        fmaxf
#define ZSL_POW        powf
#define ZSL_EXP        expf
#define ZSL_LOG        logf
#define ZSL_LOG10      log10f
#define ZSL_SQRT       sqrtf
#define ZSL_SIN        sinf
#define ZSL_COS        cosf
#define ZSL_TAN        tanf
#define ZSL_ASIN       asinf
#define ZSL_ACOS       acosf
#define ZSL_ATAN       atanf
#define ZSL_ATAN2      atan2f
#define ZSL_SINH       sinhf
#define ZSL_COSH       coshf
#define ZSL_TANH       tanhf
#define ZSL_ERF        erff
#define ZSL_FMA        fmaf
#define ZSL_EXPM1      expm1f
#else
#define ZSL_CEIL       ceil
#define ZSL_FLOOR      floor
#define ZSL_ROUND      round
#define ZSL_ABS        fabs
#define ZSL_MIN        fmin
#define ZSL_MAX        fmax
#define ZSL_POW        pow
#define ZSL_EXP        exp
#define ZSL_LOG        log
#define ZSL_LOG10      log10
#define ZSL_SQRT       sqrt
#define ZSL_SIN        sin
#define ZSL_COS        cos
#define ZSL_TAN        tan
#define ZSL_ASIN       asin
#define ZSL_ACOS       acos
#define ZSL_ATAN       atan
#define ZSL_ATAN2      atan2
#define ZSL_SINH       sinh
#define ZSL_COSH       cosh
#define ZSL_TANH       tanh
#define ZSL_ERF        erf
#define ZSL_FMA        fma
#define ZSL_EXPM1      expm1
#endif


/* TODO: Define common errors like shape mismatch, etc. */

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_H_ */
