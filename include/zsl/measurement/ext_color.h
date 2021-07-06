/*
 * Copyright (c) 2021 Linaro
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @addtogroup MEASUREMENT_COLOR Extended Type: Color
 *
 * @brief Extended meassurement types for ZSL_MES_TYPE_COLOR.
 *
 * @ingroup MES_STRUCTS
 *  @{ */

/**
 * @file
 * @brief Extended measuremeent type definitions for ZSL_MES_TYPE_COLOR.
 */

#ifndef ZSL_MEASUREMENT_EXT_COLOR_H__
#define ZSL_MEASUREMENT_EXT_COLOR_H__

#include <zsl/zsl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Extended measuremnt types for ZSL_MES_TYPE_COLOR (8-bit).
 *
 * Memory map:
 *   - 0 = RGB+A 8-bit integers (32-bit color)
 *   - 0x10..0x1F = RGB+A triplets
 *   - 0x30..0x4F = CIE values
 *   - 0x50..0xFE = Reserved
 */
enum zsl_mes_ext_color {
	/**
	 * @brief Default value for color is RGBA 8-bit integers.
	 */
	ZSL_MES_EXT_TYPE_COLOR_UNDEFINED            = 0,

	/**
	 * @brief RGBA 8-bit integers.
	 */
	ZSL_MES_EXT_TYPE_COLOR_RGBA8                = 0x10,

	/**
	 * @brief RGBA 16-bit integers.
	 *
	 */
	ZSL_MES_EXT_TYPE_COLOR_RGBA16               = 0x11,

	/**
	 * @brief RGBA 32-bit floating point values, 0..1.0 range.
	 */
	ZSL_MES_EXT_TYPE_COLOR_RGBAF                = 0x12,

	/**
	 * @brief CIE1931 XYZ tristimulus.
	 */
	ZSL_MES_EXT_TYPE_COLOR_CIE1931_XYZ          = 0x30,

	/**
	 * @brief CIE1931 xyY chromaticity.
	 */
	ZSL_MES_EXT_TYPE_COLOR_CIE1931_XYY          = 0x31,

	/**
	 * @brief CIE1960 UCS chromaticity.
	 */
	ZSL_MES_EXT_TYPE_COLOR_CIE1960_UCS          = 0x32,

	/**
	 * @brief CIE1976 UCS chromaticity.
	 */
	ZSL_MES_EXT_TYPE_COLOR_CIE1976_UCS          = 0x33,

	/**
	 * @brief CIE1960 CCT (Duv = 0.0).
	 */
	ZSL_MES_EXT_TYPE_COLOR_CIE1960_CCT          = 0x34,

	/**
	 * @brief CIE1960 CCT and Duv.
	 */
	ZSL_MES_EXT_TYPE_COLOR_CIE1960_CCT_DUV      = 0x35,
};

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* ZSL_MEASUREMENT_EXT_COLOR_H__ */
