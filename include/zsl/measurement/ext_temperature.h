/*
 * Copyright (c) 2021 Linaro
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @addtogroup MEASUREMENT_TEMP Extended Type: Temperature
 *
 * @brief Extended measurement types for ZSL_MES_TYPE_TEMPERATURE.
 *
 * @ingroup MES_STRUCTS
 *  @{ */

/**
 * @file
 * @brief Extended measurement type definitions for ZSL_MES_TYPE_TEMPERATURE.
 */

#ifndef ZSL_MEASUREMENT_EXT_TEMP_H__
#define ZSL_MEASUREMENT_EXT_TEMP_H__

#include <zsl/zsl.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Extended measurement types for ZSL_MES_TYPE_TEMPERATURE (8-bit). */
enum zsl_mes_ext_temperature {
	/**
	 * @brief Default value for light is illuminance (lux).
	 */
	ZSL_MES_EXT_TYPE_TEMP_UNDEFINED = 0,

	/**
	 * @brief Ambient temperature.
	 */
	ZSL_MES_EXT_TYPE_TEMP_AMBIENT   = 1,

	/**
	 * @brief Die temperature.
	 */
	ZSL_MES_EXT_TYPE_TEMP_DIE       = 2,

	/**
	 * @brief Object temperature.
	 */
	ZSL_MES_EXT_TYPE_TEMP_OBJECT    = 3,
};

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* ZSL_MEASUREMENT_EXT_TEMP_H__ */
