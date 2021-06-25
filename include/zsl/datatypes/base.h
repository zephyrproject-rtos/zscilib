/*
 * Copyright (c) 2021 Kevin Townsend
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * \defgroup DATATYPES Data Types
 *
 * @brief SI data types used to represent data samples
 */

/**
 * @file
 * @brief Standard data type definitions for zscilib.
 *
 * This file contains a list of standard (SI) data types used to represent
 * data samples or values in zscilib.
 */

#ifndef ZEPHYR_INCLUDE_ZSL_DATATYPES_BASE_H_
#define ZEPHYR_INCLUDE_ZSL_DATATYPES_BASE_H_

#include <zsl/zsl.h>
#include <zsl/datatypes/si.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 
 * TODO: A macro and lookup table is required to associate each data 'type'
 * with the correct SI unit.
 */ 

/**
 * @brief Base data type. An 8-bit value representing the family of data
 *        types used. This base type can be further specified from it's
 *        default type by using of an 8-bit extended data type that is
 *        specific to it's parent 'base' value.
 *
 * Memory map:
 *   - 0 = Undefined
 *   - 0x10..0x7F = Standard data types
 *   - 0x80..0xEF = Reserved
 *   - 0xF0..0xFE = User-defined base data types
 */
enum zsl_dt_base {
	ZSL_DT_BASE_UNDEFINED           = 0,

	ZSL_DT_BASE_AREA                = 0x10,
	ZSL_DT_BASE_ACCELERATION        = 0x11, /**< m/s^2: linear, gravity, etc. */
	ZSL_DT_BASE_AMPLITUDE           = 0x12, /**< 0..1.0 */
	ZSL_DT_BASE_CAPACITANCE         = 0x13, /**< uF */
	ZSL_DT_BASE_COLOR               = 0x14, /**< See extended type */
	ZSL_DT_BASE_COORDINATES         = 0x15,
	ZSL_DT_BASE_CURRENT             = 0x16, /**< mA */
	ZSL_DT_BASE_DIMENSION           = 0x17, /**< cm (length, width, distance, etc.) Name??? */
	ZSL_DT_BASE_FREQUENCY           = 0x18, /**< kHz */
	ZSL_DT_BASE_HUMIDITY            = 0x19, /**< relative humidity in percent */
	ZSL_DT_BASE_INDUCTANCE          = 0x1A, /**< nH? */
	ZSL_DT_BASE_LIGHT               = 0x1B, /**< Lux */
	ZSL_DT_BASE_MAGNETIC_FIELD      = 0x1C, /**< micro-Tesla */
	ZSL_DT_BASE_MASS                = 0x1D, /**< Grams */
	ZSL_DT_BASE_MOMENTUM            = 0x1E, /**< Angular, Linear, Inertia */
	ZSL_DT_BASE_ORIENTATION         = 0x1F,
	ZSL_DT_BASE_PHASE               = 0x20, /**< radians? */
	ZSL_DT_BASE_PRESSURE            = 0x21, /**< hectopascal (hPa) */
	ZSL_DT_BASE_RESISTANCE          = 0x22, /**< Ohms */
	ZSL_DT_BASE_SOUND               = 0x23,
	ZSL_DT_BASE_TEMPERATURE         = 0x24, /**< Celsius */
	ZSL_DT_BASE_TIME                = 0x25, /**< Default = Epoch, Duration? */
	ZSL_DT_BASE_VELOCITY            = 0x26,
	ZSL_DT_BASE_VOLTAGE             = 0x27, /**< mV? */

	ZSL_DT_BASE_USER_1              = 0xF0, /**< User-defined value 1. */
	ZSL_DT_BASE_USER_2              = 0xF1, /**< User-defined value 2. */
	ZSL_DT_BASE_USER_3              = 0xF2, /**< User-defined value 3. */
	ZSL_DT_BASE_USER_4              = 0xF3, /**< User-defined value 4. */
	ZSL_DT_BASE_USER_5              = 0xF4, /**< User-defined value 5. */
	ZSL_DT_BASE_USER_6              = 0xF5, /**< User-defined value 6. */
	ZSL_DT_BASE_USER_7              = 0xF6, /**< User-defined value 7. */
	ZSL_DT_BASE_USER_8              = 0xF7, /**< User-defined value 8. */
	ZSL_DT_BASE_USER_9              = 0xF8, /**< User-defined value 9. */
	ZSL_DT_BASE_USER_10             = 0xF9, /**< User-defined value 10. */
	ZSL_DT_BASE_USER_11             = 0xFA, /**< User-defined value 11. */
	ZSL_DT_BASE_USER_12             = 0xFB, /**< User-defined value 12. */
	ZSL_DT_BASE_USER_13             = 0xFC, /**< User-defined value 13. */
	ZSL_DT_BASE_USER_14             = 0xFD, /**< User-defined value 14. */
	ZSL_DT_BASE_USER_15             = 0xFE, /**< User-defined value 15. */

	ZSL_DT_BASE_MAX                 = 0xFF  /** Reserved. */
};

/**
 * @brief Defines a specific data type for a data sample, based on
 *        an 8-bit base type, and an optional 8-bit extended type.
 *
 * Using a base and extended data type pair allows for specialisation
 * from the default unit and scale associated with just the base type. It
 * should be possible to represent any reasonably standard unit and scale
 * using these value pairs in a 16-bit space.
 *
 * @note When the extended type is left at 0, the base type's default unit and
 *       scale will be used (mA for ZSL_DT_BASE_CURRENT, for example).
 */
struct zsl_dt_type {
	union {
		struct {
			/**
			 * @brief The base data type, which will be associated with
			 * a specific default unit and scale.
			 */
			uint8_t base_type;

			/**
			 * @brief The extended data type, which allows for specialisation
			 * from the base type. This field must be interpretted relative to
			 * the sample's base_type.
			 *
			 * Leaving this value at 0 indicates that the base_type's default
			 * unit and scale should be used.
			 */
			uint8_t ext_type;
		};
		/**
		 * @brief Represents the base and extended data types. Must be
		 * in little-endian format.
		 */
		uint16_t type;
	};
};

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_DATATYPES_BASE_H_ */

/** @} */ /* End of DATATYPES group */
