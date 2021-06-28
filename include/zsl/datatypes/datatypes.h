/*
 * Copyright (c) 2021 Kevin Townsend
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * \defgroup DATATYPES Datatypes
 *
 * @brief SI-based measurement classification system
 * 
 * \ingroup DATATYPES
 *  @{
 */

#ifndef ZEPHYR_INCLUDE_ZSL_DATATYPES_H_
#define ZEPHYR_INCLUDE_ZSL_DATATYPES_H_

#include <zsl/zsl.h>
#include <zsl/datatypes/base.h>
#include <zsl/datatypes/units.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Defines a specific measurement type for a data sample, based on
 * zsl_dt_meas_base and an appropaite extended type if required.
 *
 * Using a base and extended value pair for measurement type allows for
 * specialisation from the default (base) type. It should be possible to
 * represent any reasonably standard measurement using these value pairs in a
 * 16-bit space.
 *
 * @note Must be in little-endian format.
 *
 * @note When the extended type is set to 0, the base type's default
 *       unit will be used (ZSL_DT_UNIT_SI_AMPERE for ZSL_DT_MEAS_BASE_CURRENT,
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
			 * @brief The amount to scale the value up or down (10^n),
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

#endif /* ZEPHYR_INCLUDE_ZSL_DATATYPES_H_ */

/** @} */ /* End of DATATYPES group */
