/*
 * Copyright (c) 2021 Kevin Townsend
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @addtogroup DATATYPES_BASE Base
 *
 * @brief Base datatypes.
 *
 * \ingroup DATATYPES
 *  @{
 */

/**
 * @file
 * @brief Standard data type definitions for zscilib.
 *
 * This file contains a list of standard data types used to represent
 * data samples or measurement values in zscilib.
 */

#ifndef ZEPHYR_INCLUDE_ZSL_DATATYPES_BASE_H_
#define ZEPHYR_INCLUDE_ZSL_DATATYPES_BASE_H_

#include <zsl/zsl.h>
#include <zsl/datatypes/units.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Base data type.
 *
 * An 8-bit value with a default unit type representing a family or class of
 * measurements or data samples.
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
 *   - 0x10..0x7F = Standard data types
 *   - 0x80..0xEF = Reserved
 *   - 0xF0..0xFE = User-defined base data types
 */
enum zsl_dt_meas_base {
	ZSL_DT_MEAS_BASE_UNDEFINED      = 0,

	ZSL_DT_MEAS_BASE_AREA           = 0x10, /**< ZSL_DT_UNIT_SI_METERS_2 */
	ZSL_DT_MEAS_BASE_ACCELERATION   = 0x11, /**< ZSL_DT_UNIT_SI_METER_PER_SECOND_2 (linear, gravity) */
	ZSL_DT_MEAS_BASE_AMPLITUDE      = 0x12, /**< ZSL_DT_UNIT_SI_INTERVAL */
	ZSL_DT_MEAS_BASE_CAPACITANCE    = 0x13, /**< ZSL_DT_UNIT_SI_FARAD */
	ZSL_DT_MEAS_BASE_COLOR          = 0x14, /**< See extended type */
	ZSL_DT_MEAS_BASE_COORDINATES    = 0x15, /**< XY vector? */
	ZSL_DT_MEAS_BASE_CURRENT        = 0x16, /**< ZSL_DT_UNIT_SI_AMPERE */
	ZSL_DT_MEAS_BASE_DIMENSION      = 0x17, /**< ZSL_DT_UNIT_SI_METER (length, width, radius, distance, etc.) */
	ZSL_DT_MEAS_BASE_FREQUENCY      = 0x18, /**< ZSL_DT_UNIT_SI_HERTZ */
	ZSL_DT_MEAS_BASE_HUMIDITY       = 0x19, /**< ZSL_DT_UNIT_SI_RELATIVE_HUMIDITY */
	ZSL_DT_MEAS_BASE_INDUCTANCE     = 0x1A, /**< ZSL_DT_UNIT_SI_HENRY */
	ZSL_DT_MEAS_BASE_LIGHT          = 0x1B, /**< ZSL_DT_UNIT_SI_LUX */
	ZSL_DT_MEAS_BASE_MAGNETIC_FIELD = 0x1C, /**< ZSL_DT_UNIT_SI_TESLA */
	ZSL_DT_MEAS_BASE_MASS           = 0x1D, /**< ZSL_DT_UNIT_SI_KILOGRAM */
	ZSL_DT_MEAS_BASE_MOMENTUM       = 0x1E, /**< Angular, Linear, Inertia */
	ZSL_DT_MEAS_BASE_ORIENTATION    = 0x1F, /**< XYZ vector */
	ZSL_DT_MEAS_BASE_PHASE          = 0x20, /**< ZSL_DT_UNIT_SI_RADIAN */
	ZSL_DT_MEAS_BASE_PRESSURE       = 0x21, /**< ZSL_DT_UNIT_SI_PASCAL */
	ZSL_DT_MEAS_BASE_RESISTANCE     = 0x22, /**< ZSL_DT_UNIT_SI_OHM */
	ZSL_DT_MEAS_BASE_SOUND          = 0x23, /**< ZSL_DT_UNIT_SI_HERTZ */
	ZSL_DT_MEAS_BASE_TEMPERATURE    = 0x24, /**< ZSL_DT_UNIT_SI_DEGREE_CELSIUS */
	ZSL_DT_MEAS_BASE_TIME           = 0x25, /**< ZSL_DT_UNIT_SI_SECOND */
	ZSL_DT_MEAS_BASE_VELOCITY       = 0x26, /**< ZSL_DT_UNIT_SI_METERS_3_SECOND */
	ZSL_DT_MEAS_BASE_VOLTAGE        = 0x27, /**< ZSL_DT_UNIT_SI_VOLT */
	ZSL_DT_MEAS_BASE_VOLUME         = 0x28, /**< ZSL_DT_UNIT_SI_METERS_3 */
	ZSL_DT_MEAS_BASE_ACIDITY        = 0x29, /**< ZSL_DT_UNIT_SI_PH */
	ZSL_DT_MEAS_BASE_CONDUCTIVITY   = 0x2A, /**< ZSL_DT_UNIT_SI_SIEMENS_PER_METER */
	ZSL_DT_MEAS_BASE_FORCE          = 0x2B, /**< ZSL_DT_UNIT_SI_NEWTON */
	ZSL_DT_MEAS_BASE_ENERGY         = 0x2C, /**< ZSL_DT_UNIT_SI_JOULE */

	ZSL_DT_MEAS_BASE_USER_1         = 0xF0, /**< User-defined value 1. */
	ZSL_DT_MEAS_BASE_USER_2         = 0xF1, /**< User-defined value 2. */
	ZSL_DT_MEAS_BASE_USER_3         = 0xF2, /**< User-defined value 3. */
	ZSL_DT_MEAS_BASE_USER_4         = 0xF3, /**< User-defined value 4. */
	ZSL_DT_MEAS_BASE_USER_5         = 0xF4, /**< User-defined value 5. */
	ZSL_DT_MEAS_BASE_USER_6         = 0xF5, /**< User-defined value 6. */
	ZSL_DT_MEAS_BASE_USER_7         = 0xF6, /**< User-defined value 7. */
	ZSL_DT_MEAS_BASE_USER_8         = 0xF7, /**< User-defined value 8. */
	ZSL_DT_MEAS_BASE_USER_9         = 0xF8, /**< User-defined value 9. */
	ZSL_DT_MEAS_BASE_USER_10        = 0xF9, /**< User-defined value 10. */
	ZSL_DT_MEAS_BASE_USER_11        = 0xFA, /**< User-defined value 11. */
	ZSL_DT_MEAS_BASE_USER_12        = 0xFB, /**< User-defined value 12. */
	ZSL_DT_MEAS_BASE_USER_13        = 0xFC, /**< User-defined value 13. */
	ZSL_DT_MEAS_BASE_USER_14        = 0xFD, /**< User-defined value 14. */
	ZSL_DT_MEAS_BASE_USER_15        = 0xFE, /**< User-defined value 15. */

	ZSL_DT_MEAS_BASE_MAX            = 0xFF  /** Reserved. */
};

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_DATATYPES_BASE_H_ */

/** @} */ /* End of DATATYPES_BASE group */
