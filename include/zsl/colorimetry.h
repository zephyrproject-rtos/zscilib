/*
 * Copyright (c) 2019-2020 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @defgroup COLORIMETRY Colorimetry
 *
 * @brief Various conversion and mathematic functions to work with different color
 * systems and values.
 */

/**
 * @file
 * @brief API header file for colorimetry in zscilib.
 *
 * This file contains the entry points to the colorimetry APIs.
 */

#ifndef ZEPHYR_INCLUDE_ZSL_COLORIMETRY_H_
#define ZEPHYR_INCLUDE_ZSL_COLORIMETRY_H_

#include <stddef.h>
#include <stdint.h>
#include <zsl/zsl.h>
#include <zsl/matrices.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup COLOR_STRUCTS Structs and Enums
 *
 * @brief Common colorimetry-related structs and enums.
 *
 * @ingroup COLORIMETRY
 *  @{ */

/**
 * @brief List of supported CIE standard observer models. All models cover
 *        the 360 nm to 830 nm range in 5 nm steps.
 *
 * CIE standard observers define a model that enables the correlation of
 * spectral data to human visual assessments of color. The older 2 degree
 * model is widely used in scientific measurements and instruments, but the
 * 10 degree model is considered a more accurate approximation of how normal
 * human vision perceives color.
 */
enum zsl_clr_obs {
	/**
	 * CIE 1931 2 degree standard observer color matching functions from
	 * 360nm to 830nm in 5 nm steps.
	 */
	ZSL_CLR_OBS_2_DEG = 0,

	/**
	 * CIE 1964 10 degree standard observer color matching functions from
	 * 360nm to 830nm in 5 nm steps.
	 */
	ZSL_CLR_OBS_10_DEG
};

/**
 * @brief List of supported CIE standard illuminants.
 */
enum zsl_clr_illum {
	/**
	 * XYZ tristimulus for the standard 'A' white point or 'illuminant',
	 * used with transmissive light measurements. This illuminant
	 * simulates tungsten-filament light with a CCT of 2856K.
	 */
	ZSL_CLR_ILLUM_A = 0,

	/**
	 * XYZ tristimulus for the standard 'B' white point or 'illuminant',
	 * used with transmissive light measurements. This illuminant
	 * simulates noon sunlight with a CCT of 4874K.
	 */
	ZSL_CLR_ILLUM_B,

	/**
	 * XYZ tristimulus for the standard 'C' white point or 'illuminant',
	 * used with transmissive light measurements. This illuminant simulates
	 * typical north sky daylight with a CCT of 6774K.
	 */
	ZSL_CLR_ILLUM_C,

	/**
	 * XYZ tristimulus for the standard D50 white point or 'illuminant',
	 * used with transmissive light measurements.
	 */
	ZSL_CLR_ILLUM_D50,

	/**
	 * XYZ tristimulus for the standard D55 white point or 'illuminant',
	 * used with transmissive light measurements.
	 */
	ZSL_CLR_ILLUM_D55,

	/**
	 * XYZ tristimulus for the standard D65 white point or 'illuminant',
	 * used with transmissive light measurements.
	 */
	ZSL_CLR_ILLUM_D65,

	/**
	 * XYZ tristimulus for the standard 'E' white point or 'illuminant',
	 * used with transmissive light measurements. This illuminant is an
	 * 'equal-energy radiator' and a theoretical reference value.
	 */
	ZSL_CLR_ILLUM_E,

	/**
	 * XYZ tristimulus for the profile connection space (PCS) illuminant
	 * used in ICC profiles.
	 */
	ZSL_CLR_ILLUM_ICC
};

/**
 * @brief List of supported CIE luminous efficiency functions. All functions
 * cover the 380 nm to 780 nm range in 5 nm steps.
 *
 * Luminous efficiency functions enable conversion from radiometric units,
 * representing radiant or electromagnetic energy, into photometric units,
 * representing luminous energy as perceived by the human eye.
 *
 * For example:
 *
 * Irradiance is a radiometric unit usually represented in W/m^2, and has a
 * direct relationship to energy (watts). It is a measure of the
 * electromagnetic energy within a specified area, visible or not.
 *
 * Luminance is a photometric unit, and indicates luminous intensity per unit
 * area, as perceived by normal human vision, and is usually represented in
 * cd/m^2.
 *
 * Irradiance can be converted to luminance by means of an appropriate
 * luminous efficiency function, shaping the radiometric measurement into
 * an approximation of how a human with normal vision would perceive the
 * visible aspects of the radiometric phenomenon.
 *
 * Two major luminous efficiency functions are widely used, depending on if
 * human vision is being modelled in normal daylight conditions, where color
 * can be perceived, or in night-time conditions where sensitivity to
 * brightness is dominant.
 */
enum zsl_clr_lef {
	/**
	 * CIE 1988 Photopic Luminous Efficiency Function, which models typical
	 * human vision in well-lit conditions, and is based on the response of
	 * color-sensitive cones in the human eye.
	 */
	ZSL_CLR_LEF_CIE88_PHOTOPIC      = 0,

	/**
	 * CIE 1951 Scotopic Luminous Efficiency Function, which models typical
	 * human vision in dark conditions, and is based on the response of
	 * rods in the human eye.
	 */
	ZSL_CLR_LEF_CIE51_SCOTOPIC      = 1,
};

/**
 * @brief List of included 3x3 XYZ to RGB color space correlation matrices.
 */
enum zsl_clr_rgb_ccm {
	/** XYZ to linear sRGB (D65 white point). */
	ZSL_CLR_RGB_CCM_SRGB_D65 = 0,

	/** XYZ to linear sRGB (D50 white point). */
	ZSL_CLR_RGB_CCM_SRGB_D50,

	/** AdobeRGB98. */
	ZSL_CLR_RGB_CCM_ADOBERGB98,

	/** Sony S-Gamut3.cine D65. */
	ZSL_CLR_RGB_CCM_SONY_SGAMUT3_CINE,

	/** NTSC. */
	ZSL_CLR_RGB_CCM_NTSC,

	/** PAL/SECAM. */
	ZSL_CLR_RGB_CCM_PAL_SECAM,

	/** ITU-R BT.709. */
	ZSL_CLR_RGB_CCM_BT_709,

	/** ITU-R BT.2020. */
	ZSL_CLR_RGB_CCM_BT_2020,

	/** ACES Primaries #0 (AP0). */
	ZSL_CLR_RGB_CCM_ACES_P0,

	/** ACES Primaries #1 (AP1). */
	ZSL_CLR_RGB_CCM_ACES_P1,

	/** DCI-P3. */
	ZSL_CLR_RGB_CCM_DCI_P3,

	/** DCI-P3+. */
	ZSL_CLR_RGB_CCM_DCI_P3_PLUS,

	/** CIE linear RGB. */
	ZSL_CLR_RGB_CCM_CIE_RGB,
};

/**
 * @brief The algorithm to use when converting a CIE 1960 UV value to
 * CCT, Duv.
 */
enum zsl_clr_uv_cct_method {
	/**
	 * @brief McCamy's approximation.
	 *
	 * McCamy's approximation provides an approximate CCT using a CIE 1931
	 * xyY chromaticity:
	 *
	 *   CCT(x, y) = 449 * n^3 + 3525 * n^2 + 6823.3 * n + 5520.33
	 *
	 * Where:
	 *
	 *   n = (x − 0.3320) / (0.1858 - y)
	 *
	 * This is accurate for color temperatures between ~2800K-6500K.
	 */
	ZSL_CLR_UV_CCT_METHOD_MCCAMY,

	/**
	 * @brief Yoshi Ohno 2011 CORM.
	 *
	 * Yoshi Ohno, "Calculation of CCT and Duv and Practical Conversion
	 * Formulae", CORM 2011.
	 *
	 * Highly accurate between 2,000 and 20,000 K, making it the ideal
	 * choice for artifical lighting.
	 */
	ZSL_CLR_UV_CCT_METHOD_OHNO2011,

	/**
	 * @brief Yoshi Ohno 2014.
	 *
	 * Source: Yoshi Ohno (2014)
	 *         Practical Use and Calculation of CCT and Duv
	 *         LEUKOS, 10:1, 47-55, DOI: 10.1080/15502724.2014.839020
	 *
	 * Highly accurate between 2,600 and 20,000 K, making it the ideal
	 * choice for artifical lighting.
	 */
	ZSL_CLR_UV_CCT_METHOD_OHNO2014
};

/**
 * @brief CIE 1931 XYZ tristimulus values.
 */
struct zsl_clr_xyz {
	/** The X component of the XYZ tristimulus. */
	zsl_real_t xyz_x;
	/** The Y component of the XYZ tristimulus. */
	zsl_real_t xyz_y;
	/** The Z component of the XYZ tristimulus. */
	zsl_real_t xyz_z;
	/** The standard observer model this data is based upon. */
	enum zsl_clr_obs observer;
	/** The reference illuminant for this data (default = ZSL_CLR_ILLUM_A). */
	enum zsl_clr_illum illuminant;
	/** If non-zero, indicates that the #xyz_x value is invalid. */
	uint8_t x_invalid : 1;
	/** If non-zero, indicates that the #xyz_y value is invalid. */
	uint8_t y_invalid : 1;
	/** If non-zero, indicates that the #xyz_z value is invalid. */
	uint8_t z_invalid : 1;
};

/**
 * @brief CIE 1931 xyY chromaticity coordinates.
 */
struct zsl_clr_xyy {
	/** The x component of the xyY chromaticity coordinates. */
	zsl_real_t xyy_x;
	/** The y component of the xyY chromaticity coordinates. */
	zsl_real_t xyy_y;
	/** The Y component of the xyY chromaticity coordinates. */
	zsl_real_t xyy_Y;
	/** The standard observer model this data is based upon. */
	enum zsl_clr_obs observer;
	/** The reference illuminant for this data (default = ZSL_CLR_ILLUM_A). */
	enum zsl_clr_illum illuminant;
	/** If non-zero, indicates that the #xyy_x value is invalid. */
	uint8_t x_invalid : 1;
	/** If non-zero, indicates that the #xyy_y value is invalid. */
	uint8_t y_invalid : 1;
	/** If non-zero, indicates that the #xyy_Y value is invalid. */
	uint8_t Y_invalid : 1;
};

/**
 * @brief CIE 1960 UCS chromaticity coordinates.
 */
struct zsl_clr_uv60 {
	/** The u component of the CIE 1960 (u, v) chromaticity coordinates. */
	zsl_real_t uv60_u;
	/** The v component of the CIE 1960 (u, v) chromaticity coordinates. */
	zsl_real_t uv60_v;
	/** The standard observer model this data is based upon. */
	enum zsl_clr_obs observer;
	/** The reference illuminant for this data (default = ZSL_CLR_ILLUM_A). */
	enum zsl_clr_illum illuminant;
	/** If non-zero, indicates that the #uv60_u value is invalid. */
	uint8_t u_invalid : 1;
	/** If non-zero, indicates that the #uv60_v value is invalid. */
	uint8_t v_invalid : 1;
};

/**
 * @brief CIE 1976 UCS chromaticity coordinates.
 */
struct zsl_clr_uv76 {
	/** The u component of the CIE 1976 (u, v) chromaticity coordinates. */
	zsl_real_t uv76_u;
	/** The v component of the CIE 1976 (u, v) chromaticity coordinates. */
	zsl_real_t uv76_v;
	/** The standard observer model this data is based upon. */
	enum zsl_clr_obs observer;
	/** The reference illuminant for this data (default = ZSL_CLR_ILLUM_A). */
	enum zsl_clr_illum illuminant;
	/** If non-zero, indicates that the #uv76_u value is invalid. */
	uint8_t u_invalid : 1;
	/** If non-zero, indicates that the #uv76_v value is invalid. */
	uint8_t v_invalid : 1;
};

/**
 * @brief RGBA color using floating-space notation from 0.0 .. 1.0.
 */
struct zsl_clr_rgbf {
	/** The relative red components of an RGBA color (0.0 .. 1.0). */
	zsl_real_t r;
	/** The relative green components of an RGBA color (0.0 .. 1.0). */
	zsl_real_t g;
	/** The relative blue components of an RGBA color (0.0 .. 1.0). */
	zsl_real_t b;
	/** The relative alpha channel component (0.0 .. 1.0). */
	zsl_real_t a;
	/** If non-zero, indicates that the #r value is invalid. */
	uint8_t r_invalid : 1;
	/** If non-zero, indicates that the #g value is invalid. */
	uint8_t g_invalid : 1;
	/** If non-zero, indicates that the #b value is invalid. */
	uint8_t b_invalid : 1;
	/** If non-zero, indicates that the #a value is invalid. */
	uint8_t a_invalid : 1;
};

/**
 * @brief RGBA color using 8-bit values.
 */
struct zsl_clr_rgb8 {
	/** The 8-bit red components of an RGBA color. */
	uint8_t r;
	/** The 8-bit green components of an RGBA color. */
	uint8_t g;
	/** The 8-bit blue components of an RGBA color. */
	uint8_t b;
	/** The 8-bit alpha channel component. */
	uint8_t a;
	/** If non-zero, indicates that the #r value is invalid. */
	uint8_t r_invalid : 1;
	/** If non-zero, indicates that the #g value is invalid. */
	uint8_t g_invalid : 1;
	/** If non-zero, indicates that the #b value is invalid. */
	uint8_t b_invalid : 1;
	/** If non-zero, indicates that the #a value is invalid. */
	uint8_t a_invalid : 1;
};

/**
 * @brief RGBA color using 16-bit values.
 */
struct zsl_clr_rgb16 {
	/** The 16-bit red components of an RGBA color. */
	uint16_t r;
	/** The 16-bit green components of an RGBA color. */
	uint16_t g;
	/** The 16-bit blue components of an RGBA color. */
	uint16_t b;
	/** The 16-bit alpha channel component. */
	uint16_t a;
	/** If non-zero, indicates that the #r value is invalid. */
	uint8_t r_invalid : 1;
	/** If non-zero, indicates that the #g value is invalid. */
	uint8_t g_invalid : 1;
	/** If non-zero, indicates that the #b value is invalid. */
	uint8_t b_invalid : 1;
	/** If non-zero, indicates that the #a value is invalid. */
	uint8_t a_invalid : 1;
	/** The bit-depth of color samples (typically 10, 12, or 16). */
	uint8_t bit_depth;
};

/**
 * @brief Data representing a single CIE 1960 CCT, Duv value pair.
 */
struct zsl_clr_cct {
	/** @brief Correlated color temperature.
	 *
	 *  Indicates the color temperature that yields the minimum Euclidean
	 *  distance between u1,v1 and the Planckian chromaticity 'up, vp' of
	 *  the color temperature.
	 */
	zsl_real_t cct;
	/** @brief Distance of the measured chromaticity from the Planckian
	 *  locus (AKA black body curve).
	 *
	 *  A negative Duv indicates that the measured chromaticity is below
	 *  the Planckian locus (in the magenta and pink color range). A
	 *  positive Duv indicates a point above the locus (in the green or
	 *  yellow color range). */
	zsl_real_t duv;
	/** If non-zero, indicates that #cct is invalid. */
	uint8_t cct_invalid : 1;
	/** If non-zero, indicates that #duv is invalid. */
	uint8_t duv_invalid : 1;
};

/**
 * @brief CIE 1931 XYZ tristimulus values for a standard illuminant.
 */
struct zsl_clr_illum_data {
	/** The CIE standard illuminant reference identifier. */
	enum zsl_clr_illum illuminant;
	/** The CIE standard observer model used. */
	enum zsl_clr_obs observer;
	/** Display name string for the illuminant. */
	char *name;
	/** The illuminant's XYZ tristimulus values. */
	struct {
		/** The CIE XYZ tristimulus X value. */
		zsl_real_t xyz_x;
		/** The CIE XYZ tristimulus Y value. */
		zsl_real_t xyz_y;
		/** The CIE XYZ tristimulus Z value. */
		zsl_real_t xyz_z;
	} data;
};

/**
 * @brief CIE 1931 XYZ tristimulus values for a standard observer model in
 * 5 nm steps from 360-830 nm (95 steps).
 */
struct zsl_clr_obs_data {
	/** The CIE standard observer model identifier. */
	enum zsl_clr_obs observer;
	/** Display name string for the standard observer model. */
	char *name;
	/**
	 * The observer model's XYZ tristimulus values in 5 nm increments
	 * from 360..830 nm (95 values).
	 */
	struct {
		/** The CIE XYZ tristimulus X value. */
		zsl_real_t xyz_x;
		/** The CIE XYZ tristimulus Y value. */
		zsl_real_t xyz_y;
		/** The CIE XYZ tristimulus Z value. */
		zsl_real_t xyz_z;
	} data[95];
};

/**
 * @brief Spectral power distribution for a specific illumination.
 *
 * This struct contains the illuminant power at given wavelength intervals.
 *
 * @b Example
 *
 * The following code defines a zsl_clr_spd instance for an illumnation
 * that was captured on a spectrograph in 20 nm intervals between 380 and
 * 780 nm, for a total of 21 data samples.
 *
 * CCT was calculated as 5983.00 K and 0.0033 Duv, with an output CIE 1931 XYZ
 * tristimulus of X: 0.9516409, Y: 1.0000000, Z: 1.0059365.
 *
 * @code
 * const struct zsl_clr_spd zsl_clr_test_spd_5983k = {
 *     .size = 21,
 *     .comps = {
 *         { .nm = 380, .value = 0.290 },
 *         { .nm = 400, .value = 0.471 },
 *         { .nm = 420, .value = 0.719 },
 *         { .nm = 440, .value = 0.826 },
 *         { .nm = 460, .value = 1.006 },
 *         { .nm = 480, .value = 0.989 },
 *         { .nm = 500, .value = 0.955 },
 *         { .nm = 520, .value = 0.910 },
 *         { .nm = 540, .value = 0.950 },
 *         { .nm = 560, .value = 0.912 },
 *         { .nm = 580, .value = 0.848 },
 *         { .nm = 600, .value = 0.814 },
 *         { .nm = 620, .value = 0.827 },
 *         { .nm = 640, .value = 0.771 },
 *         { .nm = 660, .value = 0.749 },
 *         { .nm = 680, .value = 0.766 },
 *         { .nm = 700, .value = 0.705 },
 *         { .nm = 720, .value = 0.599 },
 *         { .nm = 740, .value = 0.850 },
 *         { .nm = 760, .value = 0.699 },
 *         { .nm = 780, .value = 0.898 }
 *     }
 * };
 * @endcode
 */
struct zsl_clr_spd {
	/** The number of elements in the component array. */
	size_t size;
	struct {
		/** The wavelength in nm for this specific sample. */
		unsigned int nm;
		/** The relative value of this specific sample. */
		zsl_real_t value;
	} comps[];  /**< The spectral component data that makes up the spd. */
};

/** @} */ /* End of STRUCT group */

/**
 * @addtogroup CONV Conversion Functions
 *
 * Helper functions to convert between color systems and coordinates.
 *
 * @ingroup COLORIMETRY
 *  @{ */

/**
 * @brief Converts the supplied spectral power distribution data into it's
 * equivalent XYZ tristimulus using the specified standard observer model.
 *
 * @param spd   Pointer to the spectral power distribution data to use.
 * @param obs   The CIE standard observer model to use for the conversion.
 * @param xyz   Pointer to the placeholder for the output XYZ tristimulus.
 *
 * @returns 0 on normal execution, otherwise an appropriate error code.
 */
int zsl_clr_conv_spd_xyz(const struct zsl_clr_spd *spd, enum zsl_clr_obs obs,
			 struct zsl_clr_xyz *xyz);

/**
 * @brief Converts a CIE 1931 xyY chromaticity to its XYZ tristimulus
 *        equivalent.
 *
 * @param xyy		Pointer to the input xyY triplet.
 * @param xyz       Pointer to the output XYZ triplet.
 *
 * @return 0 on success, error code on failure.
 */
int zsl_clr_conv_xyy_xyz(struct zsl_clr_xyy *xyy, struct zsl_clr_xyz *xyz);

/**
 * @brief Converts CIE 1931 XYZ tristimulus values to the xyY chromaticity
 *        equivalent.
 *
 * @param xyz       Pointer to the input XYZ triplet.
 * @param xyy		Pointer to the output xyY triplet.
 *
 * @return 0 on success, error code on failure.
 */
int zsl_clr_conv_xyz_xyy(struct zsl_clr_xyz *xyz, struct zsl_clr_xyy *xyy);

/**
 * @brief Converts a CIE 1931 xyY chromaticity to its CIE 1960 uv equivalent.
 *
 * @param xyy       Pointer to the input xyY triplet.
 * @param uv        Pointer to the output CIE 1960 u,v pair.
 *
 * @return 0 on success, error code on failure.
 */
int zsl_clr_conv_xyy_uv60(struct zsl_clr_xyy *xyy, struct zsl_clr_uv60 *uv);

/**
 * @brief Converts CIE 1931 XYZ tristimulus values to its CIE 1960 uv
 *        equivalent.
 *
 * @param xyz       Pointer to the input zsl_clr_xyz triplet.
 * @param uv        Pointer to the output CIE 1960 u,v pair.
 *
 * @return 0 on success, error code on failure.
 */
int zsl_clr_conv_xyz_uv60(struct zsl_clr_xyz *xyz, struct zsl_clr_uv60 *uv);

/**
 * @brief Converts a CIE 1960 uv value to its CIE 1931 XYZ tristimulus
 *        equivalent.
 *
 * @param uv        Pointer to the input CIE 1960 u,v pair.
 * @param xyz       Pointer to the output XYZ tristimulus.
 *
 * @return 0 on success, error code on failure.
 */
int zsl_clr_conv_uv60_xyz(struct zsl_clr_uv60 *uv, struct zsl_clr_xyz *xyz);

/**
 * @brief Converts a CIE 1960 uv value to its CIE 1931 xyY chromaticity
 *        equivalent.
 *
 * @param uv        Pointer to the input CIE 1960 u,v pair.
 * @param xyy       Pointer to the output xyY triplet.
 *
 * @return 0 on success, error code on failure.
 */
int zsl_clr_conv_uv60_xyy(struct zsl_clr_uv60 *uv, struct zsl_clr_xyy *xyy);

/**
 * @brief Converts a CIE 1960 uv value to its CIE 1976 u'v' equivalent.
 *
 * @param uv60	Pointer to the input CIE 1960 u,v pair.
 * @param uv76  Pointer to the input CIE 1976 u',v' pair.
 *
 * @return 0 on success, error code on failure.
 */
int zsl_clr_conv_uv60_uv76(struct zsl_clr_uv60 *uv60,
			   struct zsl_clr_uv76 *uv76);

/**
 * @brief Converts a CIE 1976 u'v' value to its CIE 1960 uv equivalent.
 *
 * @param uv76  Pointer to the input CIE 1976 u',v' pair.
 * @param uv60	Pointer to the input CIE 1960 u,v pair.
 *
 * @return 0 on success, error code on failure.
 */
int zsl_clr_conv_uv76_uv60(struct zsl_clr_uv76 *uv76,
			   struct zsl_clr_uv60 *uv60);

/**
 * Convert the specified color temperature to a (u,v) chromaticity.
 *
 * @param ct    The color temperature to use.
 * @param obs   The CIE standard observer model to use for the conversion.
 * @param uv    Pointer to the output (u,v) chromaticity.
 *
 * @return 0 on success, error code on failure.
 */
int zsl_clr_conv_ct_uv60(zsl_real_t ct, enum zsl_clr_obs obs,
			 struct zsl_clr_uv60 *uv);

/**
 * @brief Converts an exact CIE 1960 CCT (Duv = 0.0) to a CIE 1931 XYZ
 *        tristimulus.
 *
 * @param ct    The color temperature to use.
 * @param obs   The CIE standard observer model to use for the conversion.
 * @param xyz   Pointer to the output CIE 1931 XYZ tristimulus.
 *
 * @return 0 on success, error code on failure.
 */
int zsl_clr_conv_ct_xyz(zsl_real_t ct, enum zsl_clr_obs obs,
			struct zsl_clr_xyz *xyz);

/**
 * @brief Converts an exact CIE 1960 CCT (Duv = 0.0) to an 8-bit RGBA value
 *        using the supplied XYZ to RGB color space correlation matrix.
 *
 * @param ct    The color temperature to use.
 * @param obs   The CIE standard observer model to use for the conversion.
 * @param mtx   Pointer to the 3x3 XYZ to RGB color space correlation matrix.
 * @param rgb   Pointer to the output zsl_clr_rgb8 value.
 *
 * @return 0 on success, error code on failure.
 */
int zsl_clr_conv_ct_rgb8(zsl_real_t ct, enum zsl_clr_obs obs,
			 struct zsl_mtx *mtx, struct zsl_clr_rgb8 *rgb);

/**
 * @brief Converts an exact CIE 1960 CCT (Duv = 0.0) to an floating point RGBA
 *        value using the supplied XYZ to RGB color space correlation matrix.
 *
 * @param ct    The color temperature to use.
 * @param obs   The CIE standard observer model to use for the conversion.
 * @param mtx   Pointer to the 3x3 XYZ to RGB color space correlation matrix.
 * @param rgb   Pointer to the output zsl_clr_rgbf value.
 *
 * @return 0 on success, error code on failure.
 */
int zsl_clr_conv_ct_rgbf(zsl_real_t ct, enum zsl_clr_obs obs,
			 struct zsl_mtx *mtx, struct zsl_clr_rgbf *rgb);

/**
 * @brief Converts a CIE 1960 CCT and Duv pair to a CIE 1931 xyY chromaticity.
 *
 * @param cct   Pointer to CIE 1960 correlated color temperature and Duv.
 * @param obs   The CIE standard observer model to use for the conversion.
 * @param xyy   Pointer to the output CIE 1931 xyY chromaticity.
 *
 * @return 0 on success, error code on failure.
 */
int zsl_clr_conv_cct_xyy(struct zsl_clr_cct *cct, enum zsl_clr_obs obs,
			 struct zsl_clr_xyy *xyy);

/**
 * @brief Converts a CIE 1960 CCT and Duv pair to a CIE 1931 XYZ tristimulus.
 *
 * @param cct   Pointer to CIE 1960 correlated color temperature and Duv.
 * @param obs   The CIE standard observer model to use for the conversion.
 * @param xyz   Pointer to the output CIE 1931 XYZ tristimulus.
 *
 * @return 0 on success, error code on failure.
 */
int zsl_clr_conv_cct_xyz(struct zsl_clr_cct *cct, enum zsl_clr_obs obs,
			 struct zsl_clr_xyz *xyz);

/**
 * @brief Converts a CIE 1960 (u, v) pair to a CIE 1960 CCT and Duv
 *        pair using the specified conversion algorithm.
 *
 * @param method    The algoorithm to use for the conversion.
 * @param uv        Pointer to input CIE 1960 (u, v) pair.
 * @param cct       Pointer to output zsl_clr_cct.
 *
 * @return 0 on success, error code on failure.
 */
int zsl_clr_conv_uv60_cct(enum zsl_clr_uv_cct_method method,
			  struct zsl_clr_uv60 *uv, struct zsl_clr_cct *cct);

/**
 * @brief Converts a CIE 1931 XYZ tristimulus to an 8-bit RGBA value
 *        using the supplied XYZ to RGB color space correlation matrix.
 *
 * @param xyz   The input CIE 1931 XYZ tristimulus value to convert to RGB.
 * @param mtx   Pointer to the 3x3 XYZ to RGB color space correlation matrix.
 * @param rgb   Pointer to the output zsl_clr_rgb8 value.
 *
 * @return 0 on success, error code on failure.
 */
int zsl_clr_conv_xyz_rgb8(struct zsl_clr_xyz *xyz, struct zsl_mtx *mtx,
			  struct zsl_clr_rgb8 *rgb);

/**
 * @brief Converts a CIE 1931 XYZ tristimulus to an floating point RGBA value
 *        using the supplied XYZ to RGB color space correlation matrix.
 *
 * Depending on the value of CONFIG_ZSL_CLR_RGBF_BOUND_CAP, the output values
 * may be outside of the 0.0..1.0 range if the resulting value is out of
 * gamut for the specified RGB color space.
 *
 * @param xyz   The input CIE 1931 XYZ tristimulus value to convert to RGB.
 * @param mtx   Pointer to the 3x3 XYZ to RGB color space correlation matrix.
 * @param rgb   Pointer to the output zsl_clr_rgbf value.
 *
 * @return 0 on success, error code on failure.
 */
int zsl_clr_conv_xyz_rgbf(struct zsl_clr_xyz *xyz, struct zsl_mtx *mtx,
			  struct zsl_clr_rgbf *rgb);

/** @} */ /* End of CONV group */

/**
 * @addtogroup NORM Normalisation Functions
 *
 * Helper functions to normalise data sets.
 *
 * @ingroup COLORIMETRY
 *  @{ */

/**
 * @brief Normalises the supplied spectral power distribution data to a
 * 1.0 range. This function call is destructive to the input zsl_clr_spd data.
 *
 * @param spd   Pointer to the spectral power distribution data to normalise.
 *
 * @returns 0 on normal execution, otherwise an appropriate error code.
 */
int zsl_clr_norm_spd(struct zsl_clr_spd *spd);

/** @} */ /* End of NORM group */

/**
 * @addtogroup COLOR_DATA Data Access Functions
 *
 * Function to access various colorimetry datasets.
 *
 * @ingroup COLORIMETRY
 *  @{ */

/**
 * @brief   Retrieves a pointer to the zsl_clr_illum_data for the specified
 *          illuminant.
 *
 * @param obs   The standard observer associated with this #illum.
 * @param illum The illuminant whose data should be retrieved.
 * @param data  Pointer to the pointer where the const zsl_clr_illum_data
 *              should be made accessible.
 *
 * @returns 0 on normal execution, otherwise an appropriate error code.
 *
 * @b Example
 *
 * The following example shows how to use the pointer to a pointer
 * mechnaism to access illuminant A data in an resource efficient manner.
 *
 * @code
 * void
 * get_illum(void)
 * {
 *     int rc;
 *     const struct zsl_clr_illum_data *illum;
 *
 *     rc = zsl_clr_illum_get(ZSL_CLR_OBS_2_DEG, ZSL_CLR_ILLUM_A, &illum);
 *     if (rc) {
 *             // No matching illuminant found!
 *             return;
 *     }
 *
 *     printf("Illum %s - x = %f\n", illum->name, illum->data.xyz_z);
 * }
 *
 * @endcode
 */
int zsl_clr_illum_get(enum zsl_clr_obs obs, enum zsl_clr_illum illum,
		      const struct zsl_clr_illum_data **data);

/**
 * @brief   Retrieves a pointer to the zsl_clr_obs_data for the specified
 *          CIE standard observer model.
 *
 * @param obs   The standard observer whose data should be retrieved.
 * @param data  Pointer to the pointer where the const zsl_clr_obs_data should
 *              be made accessible.
 *
 * @returns 0 on normal execution, otherwise an appropriate error code.
 *
 * @b Example
 *
 * The following example shows how to use the pointer to a pointer
 * mechnaism to access standard 2 degree observer data in an resource
 * efficient manner.
 *
 * @code
 * void
 * get_stdobs(void)
 * {
 *     const struct zsl_clr_obs_data *obs;
 *
 *     zsl_clr_obs_get(ZSL_CLR_OBS_2DEG, &obs);
 *     printf("obs: %s\n", obs->name);
 * }
 * @endcode
 */
void zsl_clr_obs_get(enum zsl_clr_obs obs,
		     const struct zsl_clr_obs_data **data);

/**
 * @brief   Retrieves a pointer to the zsl_clr_spd data for the specified
 *          CIE luminous efficiency function.
 *
 * @param lef   The luminous efficiency func. whose data should be retrieved.
 * @param data  Pointer to the pointer where the const zsl_clr_spd data should
 *              be made accessible.
 */
void zsl_clr_lef_get(enum zsl_clr_lef lef, const struct zsl_clr_spd **data);

/**
 * @brief   Interpolates a single value from the specified CIE luminous
 *          efficiency function using linear interpolation.
 *
 * @param lef   The luminous efficiency function to use.
 * @param nm    The wavelength to interpolate.
 * @param val   Pointer to the interpolated value's placeholder.
 *
 * @returns 0 on normal execution, otherwise an appropriate error code.
 */
int zsl_clr_lef_lerp(enum zsl_clr_lef lef, unsigned int nm, zsl_real_t *val);

/**
 * @brief   Retrieves a pointer to a standard 3x3 XYZ to RGB color space
 *          correlation matrix.
 *
 * @param ccm   The XYZ to RGB color space correlation matrix to retrieve.
 * @param mtx   Pointer to the pointer where the const zsl_mtx data should
 *              be made accessible.
 *
 * @returns 0 on normal execution, otherwise an appropriate error code.
 */
void zsl_clr_rgbccm_get(enum zsl_clr_rgb_ccm ccm, struct zsl_mtx **mtx);

/** @} */ /* End of COLOR_DATA group */

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_COLORIMETRY_H_ */

/** @} */ /* End of colorimetry group */
