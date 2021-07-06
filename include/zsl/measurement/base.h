/*
 * Copyright (c) 2021 Linaro
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @addtogroup MEASUREMENT_BASE Base Types
 *
 * @brief Base measurement types.
 *
 * @ingroup MES_STRUCTS
 *  @{
 */

/**
 * @file
 * @brief Standard measureement type definitions for zscilib.
 *
 * This file contains a list of standard meassurement types used to represent
 * measurement values in zscilib.
 */

#ifndef ZSL_MEASUREMENT_BASE_H__
#define ZSL_MEASUREMENT_BASE_H__

#include <zsl/zsl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Base measurement type.
 *
 * An 8-bit value with a default SI unit type representing a family or class
 * of measurements.
 *
 * This base type can be further specialised from its default unit/type by
 * making use of an 8-bit extended type value, that extends or refines the
 * type of meassurements that can be represented.
 *
 * If an extended type is not specified (extended type = 0), the default
 * bases unit will be used.
 *
 * Memory map:
 *   - 0 = Undefined
 *   - 0x10..0x7F = Standard meassuremeent types
 *   - 0x80..0xEF = Reserved
 *   - 0xF0..0xFE = User-defined measurement types
 */
enum zsl_mes_type {
	ZSL_MES_TYPE_UNDEFINED          = 0,

	ZSL_MES_TYPE_AREA               = 0x10, /**< ZSL_MES_UNIT_SI_METERS_2 */
	ZSL_MES_TYPE_ACCELERATION       = 0x11, /**< ZSL_MES_UNIT_SI_METER_PER_SECOND_2 (linear, gravity) */
	ZSL_MES_TYPE_AMPLITUDE          = 0x12, /**< ZSL_MES_UNIT_SI_INTERVAL */
	ZSL_MES_TYPE_CAPACITANCE        = 0x13, /**< ZSL_MES_UNIT_SI_FARAD */
	ZSL_MES_TYPE_COLOR              = 0x14, /**< See extended type */
	ZSL_MES_TYPE_COORDINATES        = 0x15, /**< XY vector? */
	ZSL_MES_TYPE_CURRENT            = 0x16, /**< ZSL_MES_UNIT_SI_AMPERE */
	ZSL_MES_TYPE_DIMENSION          = 0x17, /**< ZSL_MES_UNIT_SI_METER (length, width, radius, distance, etc.) */
	ZSL_MES_TYPE_FREQUENCY          = 0x18, /**< ZSL_MES_UNIT_SI_HERTZ */
	ZSL_MES_TYPE_HUMIDITY           = 0x19, /**< ZSL_MES_UNIT_SI_RELATIVE_HUMIDITY */
	ZSL_MES_TYPE_INDUCTANCE         = 0x1A, /**< ZSL_MES_UNIT_SI_HENRY */
	ZSL_MES_TYPE_LIGHT              = 0x1B, /**< ZSL_MES_UNIT_SI_LUX */
	ZSL_MES_TYPE_MAGNETIC_FIELD     = 0x1C, /**< ZSL_MES_UNIT_SI_TESLA */
	ZSL_MES_TYPE_MASS               = 0x1D, /**< ZSL_MES_UNIT_SI_KILOGRAM */
	ZSL_MES_TYPE_MOMENTUM           = 0x1E, /**< Angular, Linear, Inertia */
	ZSL_MES_TYPE_ORIENTATION        = 0x1F, /**< XYZ vector */
	ZSL_MES_TYPE_PHASE              = 0x20, /**< ZSL_MES_UNIT_SI_RADIAN */
	ZSL_MES_TYPE_PRESSURE           = 0x21, /**< ZSL_MES_UNIT_SI_PASCAL */
	ZSL_MES_TYPE_RESISTANCE         = 0x22, /**< ZSL_MES_UNIT_SI_OHM */
	ZSL_MES_TYPE_SOUND              = 0x23, /**< ZSL_MES_UNIT_SI_HERTZ */
	ZSL_MES_TYPE_TEMPERATURE        = 0x24, /**< ZSL_MES_UNIT_SI_DEGREE_CELSIUS */
	ZSL_MES_TYPE_TIME               = 0x25, /**< ZSL_MES_UNIT_SI_SECOND */
	ZSL_MES_TYPE_VELOCITY           = 0x26, /**< ZSL_MES_UNIT_SI_METERS_3_SECOND */
	ZSL_MES_TYPE_VOLTAGE            = 0x27, /**< ZSL_MES_UNIT_SI_VOLT */
	ZSL_MES_TYPE_VOLUME             = 0x28, /**< ZSL_MES_UNIT_SI_METERS_3 */
	ZSL_MES_TYPE_ACIDITY            = 0x29, /**< ZSL_MES_UNIT_SI_PH */
	ZSL_MES_TYPE_CONDUCTIVITY       = 0x2A, /**< ZSL_MES_UNIT_SI_SIEMENS_PER_METER */
	ZSL_MES_TYPE_FORCE              = 0x2B, /**< ZSL_MES_UNIT_SI_NEWTON */
	ZSL_MES_TYPE_ENERGY             = 0x2C, /**< ZSL_MES_UNIT_SI_JOULE */

	ZSL_MES_TYPE_USER_1             = 0xF0, /**< User-defined value 1. */
	ZSL_MES_TYPE_USER_2             = 0xF1, /**< User-defined value 2. */
	ZSL_MES_TYPE_USER_3             = 0xF2, /**< User-defined value 3. */
	ZSL_MES_TYPE_USER_4             = 0xF3, /**< User-defined value 4. */
	ZSL_MES_TYPE_USER_5             = 0xF4, /**< User-defined value 5. */
	ZSL_MES_TYPE_USER_6             = 0xF5, /**< User-defined value 6. */
	ZSL_MES_TYPE_USER_7             = 0xF6, /**< User-defined value 7. */
	ZSL_MES_TYPE_USER_8             = 0xF7, /**< User-defined value 8. */
	ZSL_MES_TYPE_USER_9             = 0xF8, /**< User-defined value 9. */
	ZSL_MES_TYPE_USER_10            = 0xF9, /**< User-defined value 10. */
	ZSL_MES_TYPE_USER_11            = 0xFA, /**< User-defined value 11. */
	ZSL_MES_TYPE_USER_12            = 0xFB, /**< User-defined value 12. */
	ZSL_MES_TYPE_USER_13            = 0xFC, /**< User-defined value 13. */
	ZSL_MES_TYPE_USER_14            = 0xFD, /**< User-defined value 14. */
	ZSL_MES_TYPE_USER_15            = 0xFE, /**< User-defined value 15. */

	ZSL_MES_TYPE_MAX                = 0xFF  /** Reserved. */
};

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* ZSL_MEASUREMENT_BASE_H__ */
