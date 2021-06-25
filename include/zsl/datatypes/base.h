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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Standard SI units.
 *
 * Memory map:
 *   - 0 = Undefined
 *   - 0x10..0x1F = SI base units
 *   - 0x20..0x3F = SI derived units
 *   - 0x40..0xFE = Reserved
 */
enum zsl_dt_si_unit {
	ZSL_DT_SI_UNIT_UNDEFINED        = 0,

	/* 0x10..0x1F (16-31): SI Base Units. */
	ZSL_DT_SI_UNIT_SECOND           = 0x10, /**< s: time */
	ZSL_DT_SI_UNIT_METER            = 0x11, /**< m: length */
	ZSL_DT_SI_UNIT_KILOGRAM         = 0x12, /**< kg: mass */
	ZSL_DT_SI_UNIT_AMPERE           = 0x13, /**< A: electric current */
	ZSL_DT_SI_UNIT_KELVIN           = 0x14, /**< K: thermodynamic temp. */
	ZSL_DT_SI_UNIT_MOLE             = 0x15, /**< mol: Amount of substance */
	ZSL_DT_SI_UNIT_CANDELA          = 0x16, /**< cd: Luminous intensity */

	/* 0x20..0x3F (32-63): SI Derived Units. */
	ZSL_DT_SI_UNIT_RADIAN           = 0x20, /**< rad: plane angle, m/m */
	ZSL_DT_SI_UNIT_STERADIAN        = 0x21, /**< sr: solid angle, m^2/m^2 */
	ZSL_DT_SI_UNIT_HERTZ            = 0x22, /**< Hz: frequency, 1/s */
	ZSL_DT_SI_UNIT_NEWTON           = 0x23, /**< N: force, m*kg/s^2 */
	ZSL_DT_SI_UNIT_PASCAL           = 0x24, /**< Pa: pressure, stress, N/m^2 or kg/m*s^2 */
	ZSL_DT_SI_UNIT_JOULE            = 0x25, /**< J, energy, work, N*m or kg*m^2/s^2 */
	ZSL_DT_SI_UNIT_WATT             = 0x26, /**< W: power, radiant flux, J/s or kg*m^2/s^3 */
	ZSL_DT_SI_UNIT_COULOMB          = 0x27, /**< C: electric charge, A*s */
	ZSL_DT_SI_UNIT_VOLT             = 0x28, /**< V: elec. poten. diff, W/A or kg*m^2/A*s^3 */
	ZSL_DT_SI_UNIT_FARAD            = 0x29, /**< F: elec. capaticance, C/V or s^4*A^2/m^2*kg */
	ZSL_DT_SI_UNIT_OHM              = 0x2A, /**< electric resistence, V/A or kg*m^2/s^3*A^2*/
	ZSL_DT_SI_UNIT_SIEMENS          = 0x2B, /**< S: electric conductance, A/V */
	ZSL_DT_SI_UNIT_WEBER            = 0x2C, /**< Wb: magnetic flux, V*s or kg*m^2/s^2*A */
	ZSL_DT_SI_UNIT_TESLA            = 0x2D, /**< T: magn. flux dens., Wb/m^2 or kg/A*s^2*/
	ZSL_DT_SI_UNIT_HENRY            = 0x2E, /**< H: electric inductance, Wb/A or kg*m^2/s^2*A^2 */
	ZSL_DT_SI_UNIT_DEGREE_CELSIUS   = 0x2F, /**< C: Celsius temp, K */
	ZSL_DT_SI_UNIT_LUMEN            = 0x30, /**< lm: Luminous flux, cd*sr */
	ZSL_DT_SI_UNIT_LUX              = 0x31, /**< lx: illuminance, lm/m^2 */
	ZSL_DT_SI_UNIT_BECQUEREL        = 0x32, /**< Bq: radionucl. activity, 1/s */
	ZSL_DT_SI_UNIT_GRAY             = 0x33, /**< Gy: Absorbed dose, J/kg or m^2/s^2 */
	ZSL_DT_SI_UNIT_SIEVERT          = 0x34, /**< Sv: Dose equivalent, J/kg or m^2/s^2 */
	ZSL_DT_SI_UNIT_KATAL            = 0x35, /**< kat: Catamytic activ., mol/s */
};

/**
 * @brief Standard SI scales/powers.
 */
enum zsl_dt_si_scale {
	ZSL_DT_SCALE_YOTTA      = 24,   /**< Y: Septillion */
	ZSL_DT_SCALE_ZETTA      = 21,   /**< Z: Sextillion */
	ZSL_DT_SCALE_EXA        = 18,   /**< E: Quintillion */
	ZSL_DT_SCALE_PETA       = 15,   /**< P: Quadrillion */
	ZSL_DT_SCALE_TERA       = 12,   /**< T: Trillion */
	ZSL_DT_SCALE_GIGA       = 9,    /**< G: Billion */
	ZSL_DT_SCALE_MEGA       = 6,    /**< M: Million */
	ZSL_DT_SCALE_KILO       = 3,    /**< k: Thousand */
	ZSL_DT_SCALE_HECTO      = 2,    /**< h: Hundred */
	ZSL_DT_SCALE_DECA       = 1,    /**< da: Ten*/
	ZSL_DT_SCALE_NONE       = 0,    /**< One */
	ZSL_DT_SCALE_DECI       = -1,   /**< d: Tenth */
	ZSL_DT_SCALE_CENTI      = -2,   /**< c: Hundredth */
	ZSL_DT_SCALE_MILLI      = -3,   /**< m: Thousandth */
	ZSL_DT_SCALE_MICRO      = -6,   /**< u: Millionth */
	ZSL_DT_SCALE_NANO       = -9,   /**< n: Billionth */
	ZSL_DT_SCALE_PICO       = -12,  /**< p: Trillionth */
	ZSL_DT_SCALE_FEMTO      = -15,  /**< f: Quadrillionth */
	ZSL_DT_SCALE_ATTO       = -18,  /**< a: Quintillionth */
	ZSL_DT_SCALE_ZEPTO      = -21,  /**< z: Sextillionth */
	ZSL_DT_SCALE_YOCTO      = -24,  /**< y: Septillionth */
};

/**
 * @brief Defines a specific SI or standard unit for a data sample, based on
 *        an 8-bit base type, and an optional 8-bit extended type.
 *
 * Using this pair of base and extended data types allows for specialisation
 * from the default unit and scale associated with the base type. It
 * should be possible to represent any reasonably standard sample value
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
			u8_t base_type;

			/**
			 * @brief The extended data type, which allows for specialisation
			 * from the base type. This field must be interpretted relative to
			 * the sample's base_type.
			 *
			 * Leaving this value at 0 indicates that the base_type's default
			 * unit and scale should be used.
			 */
			u8_t ext_type;
		};
		/**
		 * @brief Represents the base and extended data types. Must be
		 * in little-endian format.
		 */
		u16_t type;
	};
};

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

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_DATATYPES_BASE_H_ */

/** @} */ /* End of datatypes group */
