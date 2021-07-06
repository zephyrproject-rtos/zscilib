/*
 * Copyright (c) 2021 Linaro
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @addtogroup MEASUREMENT_LIGHT Extended Type: Light
 *
 * @brief Extended measurement types for ZSL_MES_TYPE_LIGHT.
 *
 * @ingroup MES_STRUCTS
 *  @{ */

/**
 * @file
 * @brief Extended measurement type definitions for ZSL_MES_TYPE_LIGHT.
 */

#ifndef ZSL_MEASUREMENT_EXT_LIGHT_H__
#define ZSL_MEASUREMENT_EXT_LIGHT_H__

#include <zsl/zsl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Extended measurement types for ZSL_MES_TYPE_LIGHT (8-bit).
 *
 * Memory map:
 *   - 0 = Illuminance (lux)
 *   - 0x10..0x3F = Radiometric units
 *   - 0x40..0x5F = Photometric units
 *   - 0x60..0xFE = Reserved
 *
 * <b>Radiometric Units (ZSL_MES_EXT_TYPE_LIGHT_RADIO_*)</b>
 *
 * Electromagentic radiation is characterised by radiometric units, which
 * describe the physical properties of light (the number of photons, photon
 * energy, or radiant flux). These units have no relation to human vision.
 * Infrared radiation, for example, is not visible to the human eye (>780 nm)
 * but it clearly exists as a radiometric phenomenon and can be accurately
 * measured, described and analysed for scientifically significant purposes.
 *
 * <b>Photometric Units (ZSL_MES_EXT_TYPE_LIGHT_PHOTO_*)</b>
 *
 * To characterise light relative to the human eye, we need to use
 * photometric units such as luminous intensity, which represents the
 * light intensity of a source as perceived by the human eye, measured in
 * candela (cd).
 *
 * Since photometric measurements are limited to what the human eye can
 * perceive, these measurements are restricted to the visible spectrum
 * of 380 nm to 780 nm wavelengths.
 *
 * One of the most common photometric units is luminous flux, which is
 * measured in lumens (lm). It's even more common derivative is illuminance,
 * which is the luminous flux incident per a specific area. Illuminance is
 * measured in lux (which is equal to lm/m^2).
 */
enum zsl_mes_ext_light {
	/**
	 * @brief Default value for light is illuminance (lux).
	 */
	ZSL_MES_EXT_TYPE_LIGHT_UNDEFINED                         = 0,

	/**
	 * @brief Energy of electromagnetic radiation. Measured in joules (J).
	 */
	ZSL_MES_EXT_TYPE_LIGHT_RADIO_RADIANT_ENERGY              = 0x10,

	/**
	 * @brief Radiant energy per unit volume. Measured in joule per cubic
	 * meter (J/m^3).
	 */
	ZSL_MES_EXT_TYPE_LIGHT_RADIO_RADIANT_ENERGY_DENSITY      = 0x11,

	/**
	 * @brief Radiant energy emitted, reflected, transmitted or received,
	 * per unit time. Measure in watts (W), where W = J/s.
	 */
	ZSL_MES_EXT_TYPE_LIGHT_RADIO_RADIANT_FLUX                = 0x12,

	/**
	 * @brief Radiant flux per unit frequency. Measure in watts per hertz
	 * (W/Hz).
	 */
	ZSL_MES_EXT_TYPE_LIGHT_RADIO_SPECTRAL_FLUX_HZ            = 0x13,

	/**
	 * @brief Radiant flux per wavelength. Measured in watts per nanometer
	 * (W/nm).
	 */
	ZSL_MES_EXT_TYPE_LIGHT_RADIO_SPECTRAL_FLUX_NM            = 0x14,

	/**
	 * @brief Radiant flux emitted, reflected, transmitted or received, per
	 * unit solid angle. This is measured in watt per steradian (W/sr).
	 */
	ZSL_MES_EXT_TYPE_LIGHT_RADIO_RADIANT_INTENSITY           = 0x15,

	/**
	 * @brief Radiant intensity per unit frequency. Measured in watt per
	 * steradian per hertz (W/sr/Hz).
	 */
	ZSL_MES_EXT_TYPE_LIGHT_RADIO_SPECTRAL_INTENSITY_HZ       = 0x16,

	/**
	 * @brief Radiant intensity per wavelength. Measured in watt per
	 * steradian per nanometer (W/sr/nm).
	 */
	ZSL_MES_EXT_TYPE_LIGHT_RADIO_SPECTRAL_INTENSITY_NM       = 0x17,

	/**
	 * @brief Radiant flux emitted, reflected, transmitted or received by a
	 * surface, per unit solid angle per unit projected area. Measured in
	 * watt per steradian per square meter (W/sr/m^2).
	 */
	ZSL_MES_EXT_TYPE_LIGHT_RADIO_RADIANCE                    = 0x18,

	/**
	 * @brief Radiance of a surface per unit frequency. Measured in watt per
	 * steradian per square meter per hertz (W/sr/m^2/Hz).
	 */
	ZSL_MES_EXT_TYPE_LIGHT_RADIO_SPECTRAL_RADIANCE_HZ        = 0x19,

	/**
	 * @brief Radiance of a surface per wavelength. Measured in watt per
	 * steradian per square meter per nanometer (W/sr/m^2/nm)
	 */
	ZSL_MES_EXT_TYPE_LIGHT_RADIO_SPECTRAL_RADIANCE_NM        = 0x1A,

	/**
	 * @brief Radiant flux received by a surface per unit area. Measured in
	 * watt per square meter (W/m^2). AKA Flux Density.
	 */
	ZSL_MES_EXT_TYPE_LIGHT_RADIO_IRRADIANCE                  = 0x1B,

	/**
	 * @brief Irradiance of a surface per unit frequency. Measured in watt per
	 * square meter per hertz (W/m^2/Hz).
	 */
	ZSL_MES_EXT_TYPE_LIGHT_RADIO_SPECTRAL_IRRADIANCE_HZ      = 0x1C,

	/**
	 * @brief Irraidance of a surface per wavelength. Measured in watt per
	 * square meter per nanometer (W/m^2/nm). AKA Spectral Flux Density.
	 */
	ZSL_MES_EXT_TYPE_LIGHT_RADIO_SPECTRAL_IRRADIANCE_NM      = 0x1D,

	/**
	 * @brief Radiant flux leaving (emitted, reflected and transmitted by)
	 * a surface per unit area. Measured in watt per square meter (W/m^2).
	 */
	ZSL_MES_EXT_TYPE_LIGHT_RADIO_RADIOSITY                   = 0x1E,

	/**
	 * @brief Radiosity of a surface per unit frequency. Measured in watt per
	 * square meter per hertz (W/m^2/Hz).
	 */
	ZSL_MES_EXT_TYPE_LIGHT_RADIO_SPECTRAL_RADIOSITY_HZ       = 0x1F,

	/**
	 * @brief Radiosity of a surface per unit wavelength. Measured in watt per
	 * square meter per nanometer (W/m^2/nm).
	 */
	ZSL_MES_EXT_TYPE_LIGHT_RADIO_SPECTRAL_RADIOSITY_NM       = 0x20,

	/**
	 * @brief Radiant flux emitted by a surface per unit area. Measured in
	 * watt per square meter (W/m^2). AKA Radiant Emittance.
	 */
	ZSL_MES_EXT_TYPE_LIGHT_RADIO_RADIANT_EXITANCE            = 0x21,

	/**
	 * @brief Radiant exitance of a surface per unit frequency. Measured in
	 * watt per square meter per hertz (W/m^2/Hz)
	 */
	ZSL_MES_EXT_TYPE_LIGHT_RADIO_SPECTRAL_EXITANCE_HZ        = 0x22,

	/**
	 * @brief Radiant exitance of a surface per wavelength. Measured in watt
	 * per square meter per nanometer (W/m^2/nm).
	 */
	ZSL_MES_EXT_TYPE_LIGHT_RADIO_SPECTRAL_EXITANCE_NM        = 0x23,

	/**
	 * @brief Radiant energy received by a surface per unit area, or
	 * equivalently irradiance of a surface integrated over time of
	 * irradiation. Measured in joule per square meter (J/m^2).
	 */
	ZSL_MES_EXT_TYPE_LIGHT_RADIO_RADIANT_EXPOSURE            = 0x24,

	/**
	 * @brief Radiant exposure of a surface per unit area. Measured in joule
	 * per square meter per hertz (J/m^2/Hz).
	 */
	ZSL_MES_EXT_TYPE_LIGHT_RADIO_SPECTRAL_EXPOSURE_HZ        = 0x25,

	/**
	 * @brief Radiant exposure of a surface per wavelength. Measured in joule
	 * per square meter per nanometer (J/m^2/nm).
	 */
	ZSL_MES_EXT_TYPE_LIGHT_RADIO_SPECTRAL_EXPOSURE_NM        = 0x26,

	/**
	 * @brief Lumen second (AKA talbot).
	 */
	ZSL_MES_EXT_TYPE_LIGHT_PHOTO_LUM_ENERGY                  = 0x40,

	/**
	 * @brief Luminous energy per unit time. Measured in lumen (candela
	 * steradian).
	 */
	ZSL_MES_EXT_TYPE_LIGHT_PHOTO_LUM_FLUX                    = 0x41,

	/**
	 * @brief Luminous flux per unit solid angle. Measured in candela (lumen
	 * per steradian, or lm/sr).
	 */
	ZSL_MES_EXT_TYPE_LIGHT_PHOTO_LUM_INTENSITY               = 0x42,

	/**
	 * @brief Luminous flux per unit solid angle per unit projected source
	 * area. Measured in candela per square meter (cd/m^2).
	 */
	ZSL_MES_EXT_TYPE_LIGHT_PHOTO_LUMINANCE                   = 0x43,

	/**
	 * @brief Luminous flux incident on a surface. Measured in lux or lumen per
	 * square meter (lm/m^2).
	 */
	ZSL_MES_EXT_TYPE_LIGHT_PHOTO_ILLUMINANCE                 = 0x44,

	/**
	 * @brief Luminous flux emitted from a surface. Measured in lumen per
	 * square meter (lm/m^2).
	 */
	ZSL_MES_EXT_TYPE_LIGHT_PHOTO_LUM_EXITANCE                = 0x45,

	/**
	 * @brief Time-integrated luminance. Measured in lux second (lx s).
	 */
	ZSL_MES_EXT_TYPE_LIGHT_PHOTO_LUM_EXPOSURE                = 0x46,

	/**
	 * @brief Lumen second per cubic meter (lm s/m^3).
	 */
	ZSL_MES_EXT_TYPE_LIGHT_PHOTO_LUM_ENERGY_DENSITY          = 0x47,

	/**
	 * @brief Ratio of luminous flux to radiant flux. Measured in lumen per
	 * watt (lm/W).
	 */
	ZSL_MES_EXT_TYPE_LIGHT_PHOTO_LUM_EFFICACY_RAD            = 0x48,

	/**
	 * @brief Ratio of luminous flux to power consumption of a source. Measured
	 * in lumen per watt (lm/W).
	 */
	ZSL_MES_EXT_TYPE_LIGHT_PHOTO_LUM_EFFICACY_SRC            = 0x49,
};

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* ZSL_MEASUREMENT_EXT_LIGHT_H__ */
