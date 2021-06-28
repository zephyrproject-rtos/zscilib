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
 * measurements or data samples. This base type can be further specialised from
 * its default unit/type by making use of an 8-bit extended type value, that
 * extends or refines thee type of meassurements that can be represented. If an
 * extended type is not specified (extended type = 0), the default 'base' unit
 * will be used.
 *
 * Memory map:
 *   - 0 = Undefined
 *   - 0x10..0x7F = Standard data types
 *   - 0x80..0xEF = Reserved
 *   - 0xF0..0xFE = User-defined base data types
 */
enum zsl_dt_mes_base {
	ZSL_DT_MES_BASE_UNDEFINED       = 0,

	ZSL_DT_MES_BASE_AREA            = 0x10,         /**< ZSL_DT_UNIT_SI_METERS_2 */
	ZSL_DT_MES_BASE_ACCELERATION    = 0x11,         /**< ZSL_DT_UNIT_SI_METER_PER_SECOND_2 (linear, gravity) */
	ZSL_DT_MES_BASE_AMPLITUDE       = 0x12,         /**< 0..1.0 */
	ZSL_DT_MES_BASE_CAPACITANCE     = 0x13,         /**< ZSL_DT_UNIT_SI_FARAD */
	ZSL_DT_MES_BASE_COLOR           = 0x14,         /**< See extended type */
	ZSL_DT_MES_BASE_COORDINATES     = 0x15,         /**< XY vector? */
	ZSL_DT_MES_BASE_CURRENT         = 0x16,         /**< ZSL_DT_UNIT_SI_AMPERE */
	ZSL_DT_MES_BASE_DIMENSION       = 0x17,         /**< ZSL_DT_UNIT_SI_METER (length, width, radius, distance, etc.) */
	ZSL_DT_MES_BASE_FREQUENCY       = 0x18,         /**< ZSL_DT_UNIT_SI_HERTZ */
	ZSL_DT_MES_BASE_HUMIDITY        = 0x19,         /**< relative humidity in percent */
	ZSL_DT_MES_BASE_INDUCTANCE      = 0x1A,         /**< ZSL_DT_UNIT_SI_HENRY */
	ZSL_DT_MES_BASE_LIGHT           = 0x1B,         /**< ZSL_DT_UNIT_SI_LUX */
	ZSL_DT_MES_BASE_MAGNETIC_FIELD  = 0x1C,         /**< ZSL_DT_UNIT_SI_TESLA */
	ZSL_DT_MES_BASE_MASS            = 0x1D,         /**< ZSL_DT_UNIT_SI_KILOGRAM */
	ZSL_DT_MES_BASE_MOMENTUM        = 0x1E,         /**< Angular, Linear, Inertia */
	ZSL_DT_MES_BASE_ORIENTATION     = 0x1F,         /**< XYZ vector */
	ZSL_DT_MES_BASE_PHASE           = 0x20,         /**< ZSL_DT_UNIT_SI_RADIAN */
	ZSL_DT_MES_BASE_PRESSURE        = 0x21,         /**< ZSL_DT_UNIT_SI_PASCAL */
	ZSL_DT_MES_BASE_RESISTANCE      = 0x22,         /**< ZSL_DT_UNIT_SI_OHM */
	ZSL_DT_MES_BASE_SOUND           = 0x23,         /**< ZSL_DT_UNIT_SI_HERTZ */
	ZSL_DT_MES_BASE_TEMPERATURE     = 0x24,         /**< ZSL_DT_UNIT_SI_DEGREE_CELSIUS */
	ZSL_DT_MES_BASE_TIME            = 0x25,         /**< ZSL_DT_UNIT_SI_SECOND */
	ZSL_DT_MES_BASE_VELOCITY        = 0x26,         /**< ZSL_DT_UNIT_SI_METERS_3_SECOND */
	ZSL_DT_MES_BASE_VOLTAGE         = 0x27,         /**< ZSL_DT_UNIT_SI_VOLT */
	ZSL_DT_MES_BASE_VOLUME          = 0x28,         /**< ZSL_DT_UNIT_SI_METERS_3 */
	ZSL_DT_MES_BASE_ACIDITY         = 0x29,         /**< ZSL_DT_UNIT_SI_PH */
	ZSL_DT_MES_BASE_CONDUCTIVITY    = 0x2A,         /**< ZSL_DT_UNIT_SI_SIEMENS_PER_METER */
	ZSL_DT_MES_BASE_FORCE           = 0x2B,         /**< ZSL_DT_UNIT_SI_NEWTON */
	ZSL_DT_MES_BASE_ENERGY          = 0x2C,         /**< ZSL_DT_UNIT_SI_JOULE */

	ZSL_DT_MES_BASE_USER_1          = 0xF0,         /**< User-defined value 1. */
	ZSL_DT_MES_BASE_USER_2          = 0xF1,         /**< User-defined value 2. */
	ZSL_DT_MES_BASE_USER_3          = 0xF2,         /**< User-defined value 3. */
	ZSL_DT_MES_BASE_USER_4          = 0xF3,         /**< User-defined value 4. */
	ZSL_DT_MES_BASE_USER_5          = 0xF4,         /**< User-defined value 5. */
	ZSL_DT_MES_BASE_USER_6          = 0xF5,         /**< User-defined value 6. */
	ZSL_DT_MES_BASE_USER_7          = 0xF6,         /**< User-defined value 7. */
	ZSL_DT_MES_BASE_USER_8          = 0xF7,         /**< User-defined value 8. */
	ZSL_DT_MES_BASE_USER_9          = 0xF8,         /**< User-defined value 9. */
	ZSL_DT_MES_BASE_USER_10         = 0xF9,         /**< User-defined value 10. */
	ZSL_DT_MES_BASE_USER_11         = 0xFA,         /**< User-defined value 11. */
	ZSL_DT_MES_BASE_USER_12         = 0xFB,         /**< User-defined value 12. */
	ZSL_DT_MES_BASE_USER_13         = 0xFC,         /**< User-defined value 13. */
	ZSL_DT_MES_BASE_USER_14         = 0xFD,         /**< User-defined value 14. */
	ZSL_DT_MES_BASE_USER_15         = 0xFE,         /**< User-defined value 15. */

	ZSL_DT_MES_BASE_MAX             = 0xFF          /** Reserved. */
};

/**
 * @brief Defines a specific measurement type for a data sample, based on
 * zsl_dt_mes_base and an appropaite extended type if required.
 *
 * Using a base and extended value pair for measurement type allows for
 * specialisation from the default (base) type. It should be possible to
 * represent any reasonably standard measurement using these value pairs in a
 * 16-bit space.
 *
 * @note Must be in little-endian format.
 *
 * @note When the extended type is set to 0, the base type's default
 *       unit will be used (ZSL_DT_UNIT_SI_AMPERE for ZSL_DT_MES_BASE_CURRENT,
 *       for example).
 */
struct zsl_dt_meas_type {
	union {
		struct {
			/**
			 * @brief The base measurement type, which will be associated with
			 * a default SI unit and scale.
			 */
			uint8_t base;

			/**
			 * @brief The extended measurement type, which allows for
			 * specialisation of the base type. This field must be interpretted
			 * relative to the sample's base type.
			 *
			 * Leaving this value at 0 indicates that the base type's
			 * default SI unit and scale should be used.
			 */
			uint8_t ext;
		};

		/**
		 * @brief Represents the base and extended measurement types.
		 */
		uint16_t full;
	};
};

/**
 * @brief Defines the SI unit and default scale, underlying C data type and
 * optional scale factor for a data sample.
 *
 * This value is composed of a 16-bit SI unit and default scale indicator
 * (si_unit), an 8-bit indicator of the underlying C type the unit is based
 * upon (ctype), and an optional signed 8-bit scale indicator for that
 * unit (scale_factor), which can be used to indicte that the data sample's
 * value was shifted up or down the scale factor from whatever default scale is
 * represented by si_unit (for example: -3 or ZSL_DT_UNIT_SCALE_MILLI to
 * represent mA with a default SI unit of ZSL_DT_UNIT_SI_AMPERE).
 *
 * Indicating the SI unit, C data type and optional scale factor alongside the
 * measurement type allows for programmatic knowledge of the size and
 * interpretation of the sample's representation in meomry, and the ability to
 * choose a more appropriate scale depending on the source of the measurement
 * (uA versus A, for example).
 *
 * @note Must be in little-endian format.
 */
struct zsl_dt_unit_type {
	union {
		struct {
			/**
			 * @brief The SI unit and default scale used for this measurement.
			 * Must be a member of zsl_dt_unit_si.
			 */
			uint16_t si_unit;

			/**
			 * @brief The data type that this SI unit is represented by in C.
			 * Must be a member of zsl_dt_unit_ctype.
			 *
			 * This field can be used to determine how many bytes are required
			 * to represent this measurement value, and how to nterpret the
			 * value in memory.
			 */
			uint8_t ctype;

			/**
			 * @brief The amount to scale the value up or down (as a power),
			 * starting from the unit and scale indicated by si_unit.
			 * Typically, but not necessarily a member of zsl_dt_unit_scale.
			 */
			int8_t scale_factor;
		};

		/**
		 * @brief 32-bit representation of si_unit, ctype and scale_factor.
		 */
		uint32_t full;
	};
};

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_DATATYPES_BASE_H_ */

/** @} */ /* End of DATATYPES group */
