/*
 * Copyright (c) 2021 Kevin Townsend
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @addtogroup DATATYPES_SI SI units and scales
 *
 * @brief Enums for SI base, derived and combined units and scales.
 *
 * \ingroup DATATYPES
 *  @{ */

/**
 * @file
 * @brief SI unit and scale definitions for zscilib.
 *
 * This file contains a list of standard (SI) units and scales used to
 * represent data samples or values in zscilib.
 */

#ifndef ZEPHYR_INCLUDE_ZSL_DATATYPES_SI_H_
#define ZEPHYR_INCLUDE_ZSL_DATATYPES_SI_H_

#include <zsl/zsl.h>

#ifdef __cplusplus
extern "C" {
#endif

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
 * @brief Standard SI units (16-bits).
 * 
 * @note Base and derived SI units can be represented in 8-bits, while
 * combined units require 16-bits to represent.
 *
 * Memory map:
 *   - 0              = Undefined
 *   - 0x0010..0x001F = SI base units
 *   - 0x0020..0x003F = SI derived units
 *   - 0x0040..0x00FF = Reserved
 *   - 0x0100..0xFEFF = Combined units
 *   - 0xFF00..0xFFFE = User defined units
 */
enum zsl_dt_si_unit {
	ZSL_DT_SI_UNIT_UNDEFINED                                = 0,

	/* 0x0010..0x001F (16-31): SI Base Units. */
	ZSL_DT_SI_UNIT_AMPERE                                   = 0x10, /**< A: electric current */
	ZSL_DT_SI_UNIT_CANDELA                                  = 0x11, /**< cd: Luminous intensity */
	ZSL_DT_SI_UNIT_KELVIN                                   = 0x12, /**< K: thermodynamic temp. */
	ZSL_DT_SI_UNIT_KILOGRAM                                 = 0x13, /**< kg: mass */
	ZSL_DT_SI_UNIT_METER                                    = 0x14, /**< m: length */
	ZSL_DT_SI_UNIT_MOLE                                     = 0x15, /**< mol: Amount of substance */
	ZSL_DT_SI_UNIT_SECOND                                   = 0x16, /**< s: time */

	/* 0x0020..0x003F (32-63): SI Derived Units. */
	ZSL_DT_SI_UNIT_BECQUEREL                                = 0x20, /**< Bq: radionucl. activity, 1/s */
	ZSL_DT_SI_UNIT_COULOMB                                  = 0x21, /**< C: electric charge, A*s */
	ZSL_DT_SI_UNIT_DEGREE_CELSIUS                           = 0x22, /**< C: Celsius temp, K */
	ZSL_DT_SI_UNIT_FARAD                                    = 0x23, /**< F: elec. capaticance, C/V or s^4*A^2/m^2*kg */
	ZSL_DT_SI_UNIT_GRAY                                     = 0x24, /**< Gy: Absorbed dose, J/kg or m^2/s^2 */
	ZSL_DT_SI_UNIT_HENRY                                    = 0x25, /**< H: electric inductance, Wb/A or kg*m^2/s^2*A^2 */
	ZSL_DT_SI_UNIT_HERTZ                                    = 0x26, /**< Hz: frequency, 1/s */
	ZSL_DT_SI_UNIT_JOULE                                    = 0x27, /**< J, energy, work, N*m or kg*m^2/s^2 */
	ZSL_DT_SI_UNIT_KATAL                                    = 0x28, /**< kat: Catamytic activ., mol/s */
	ZSL_DT_SI_UNIT_LUMEN                                    = 0x29, /**< lm: Luminous flux, cd*sr */
	ZSL_DT_SI_UNIT_LUX                                      = 0x2A, /**< lx: illuminance, lm/m^2 */
	ZSL_DT_SI_UNIT_NEWTON                                   = 0x2B, /**< N: force, m*kg/s^2 */
	ZSL_DT_SI_UNIT_OHM                                      = 0x2C, /**< electric resistence, V/A or kg*m^2/s^3*A^2*/
	ZSL_DT_SI_UNIT_PASCAL                                   = 0x2D, /**< Pa: pressure, stress, N/m^2 or kg/m*s^2 */
	ZSL_DT_SI_UNIT_RADIAN                                   = 0x2E, /**< rad: plane angle, m/m */
	ZSL_DT_SI_UNIT_SIEMENS                                  = 0x2F, /**< S: electric conductance, A/V */
	ZSL_DT_SI_UNIT_SIEVERT                                  = 0x30, /**< Sv: Dose equivalent, J/kg or m^2/s^2 */
	ZSL_DT_SI_UNIT_STERADIAN                                = 0x31, /**< sr: solid angle, m^2/m^2 */
	ZSL_DT_SI_UNIT_TESLA                                    = 0x32, /**< T: magn. flux dens., Wb/m^2 or kg/A*s^2*/
	ZSL_DT_SI_UNIT_VOLT                                     = 0x33, /**< V: elec. poten. diff, W/A or kg*m^2/A*s^3 */
	ZSL_DT_SI_UNIT_WATT                                     = 0x34, /**< W: power, radiant flux, J/s or kg*m^2/s^3 */
	ZSL_DT_SI_UNIT_WEBER                                    = 0x35, /**< Wb: magnetic flux, V*s or kg*m^2/s^2*A */

	/* 0x1B00..0x1BFF: SL_DT_EXT_LIGHT Combined Units. */
	ZSL_DT_SI_UNIT_C_CANDELA_PER_METER_2                    = 0x1B00,       /**< cd/m^2 */
	ZSL_DT_SI_UNIT_C_JOULE_PER_METER_2                      = 0x1B01,       /**< J/m^2 */
	ZSL_DT_SI_UNIT_C_JOULE_PER_METER_2_PER_HZ               = 0x1B02,       /**< J/m^2/Hz */
	ZSL_DT_SI_UNIT_C_JOULE_PER_METER_2_PER_NM               = 0x1B03,       /**< J/m^2/nm */
	ZSL_DT_SI_UNIT_C_JOULE_PER_METER_3                      = 0x1B04,       /**< Energy density, J/m^3 */
	ZSL_DT_SI_UNIT_C_LUMEN_PER_METER_2                      = 0x1B05,       /**< lm/m^2 */
	ZSL_DT_SI_UNIT_C_LUMEN_PER_WATT                         = 0x1B06,       /**< lm/W */
	ZSL_DT_SI_UNIT_C_LUMEN_SECOND                           = 0x1B07,       /**< AKA talbot */
	ZSL_DT_SI_UNIT_C_LUMEN_SECOND_PER_METER_3               = 0x1B08,       /**< lm s/m^3 */
	ZSL_DT_SI_UNIT_C_LUX_SECOND                             = 0x1B09,       /**< lx s */
	ZSL_DT_SI_UNIT_C_WATTS_PER_HERTZ                        = 0x1B0A,       /**< W/Hz */
	ZSL_DT_SI_UNIT_C_WATTS_PER_METER_2                      = 0x1B0B,       /**< W/m^2 */
	ZSL_DT_SI_UNIT_C_WATTS_PER_METER_2_PER_HZ               = 0x1B0C,       /**< W/m^2/Hz */
	ZSL_DT_SI_UNIT_C_WATTS_PER_METER_2_PER_NM               = 0x1B0D,       /**< W/m^2/nm */
	ZSL_DT_SI_UNIT_C_WATTS_PER_NM                           = 0x1B0E,       /**< W/nm */
	ZSL_DT_SI_UNIT_C_WATTS_PER_STERADIAN                    = 0x1B0F,       /**< W/sr */
	ZSL_DT_SI_UNIT_C_WATTS_PER_STERADIAN_PER_HERTZ          = 0x1B10,       /**< W/sr/Hz */
	ZSL_DT_SI_UNIT_C_WATTS_PER_STERADIAN_PER_METER_2        = 0x1B11,       /**< W/sr/m^2 */
	ZSL_DT_SI_UNIT_C_WATTS_PER_STERADIAN_PER_METER_2_PER_HZ = 0x1B12,       /**< W/sr/m^2/Hz */
	ZSL_DT_SI_UNIT_C_WATTS_PER_STERADIAN_PER_METER_2_PER_NM = 0x1B13,       /**< W/sr/m^2/nm */
	ZSL_DT_SI_UNIT_C_WATTS_PER_STERADIAN_PER_NM             = 0x1B14,       /**< W/sr/nm */
};

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_DATATYPES_SI_H_ */

/** @} */ /* End of DATATYPES_SI group */
