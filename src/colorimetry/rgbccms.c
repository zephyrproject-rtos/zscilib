/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <errno.h>
#include <zsl/colorimetry.h>

/**
 * @brief 3x3 XYZ to RGB color space correlation matrices.
 */
static struct zsl_mtx zsl_clr_rgb_ccm_list[] = {
	/** Linear sRGB with D65 white point. */
	{ .sz_rows = 3,
	  .sz_cols = 3,
	  .data = (zsl_real_t[]) {
		  3.2404542, -1.5371385, -0.4985314,
		  -0.9692660, 1.8760108,  0.0415560,
		  0.0556434, -0.2040259,  1.0572252
	  } },

	/** Linear sRGB with D50 white point. */
	{ .sz_rows = 3,
	  .sz_cols = 3,
	  .data = (zsl_real_t[]) {
		  3.1338561, -1.6168667, -0.4906146,
		  -0.9787684, 1.9161415,  0.0334540,
		  0.0719453, -0.2289914,  1.4052427
	  } },

	/** AdobeRGB98. */
	{ .sz_rows = 3,
	  .sz_cols = 3,
	  .data = (zsl_real_t[]) {
		  2.0413690, -0.5649464, -0.3446944,
		  -0.9692660, 1.8760108,  0.0415560,
		  0.0134474, -0.1183897,  1.0154096
	  } },

	/** Sony S-Gamut3.cine D65. */
	{ .sz_rows = 3,
	  .sz_cols = 3,
	  .data = (zsl_real_t[]) {
		  1.84664243, -0.52594723, -0.21052964,
		  -0.44417115, 1.25949363,  0.14940599,
		  0.04086348,  0.01564397,  0.86837846
	  } },

	/** NTSC. */
	{ .sz_rows = 3,
	  .sz_cols = 3,
	  .data = (zsl_real_t[]) {
		  1.91008143, -0.53247794, -0.28822201,
		  -0.98463135, 1.99910001, -0.02830719,
		  0.05830945, -0.11838584, 0.89761208
	  } },

	/** PAL/SECAM. */
	{ .sz_rows = 3,
	  .sz_cols = 3,
	  .data = (zsl_real_t[]) {
		  3.06338864, -1.39340271, -0.47582802,
		  -0.96922425, 1.87592998, 0.04155423,
		  0.06787259, -0.2288382, 1.06927151
	  } },

	/* Source: Colour python library. */
	/** ITU-R BT.709. */
	{ .sz_rows = 3,
	  .sz_cols = 3,
	  .data = (zsl_real_t[]) {
		  3.24100326, -1.53739899, -0.49861587,
		  -0.96922426, 1.87592999, 0.04155422,
		  0.05563942, -0.2040112, 1.05714897
	  } },

	/* Source: Colour python library. */
	/** ITU-R BT.2020. */
	{ .sz_rows = 3,
	  .sz_cols = 3,
	  .data = (zsl_real_t[]) {
		  1.71666343, -0.35567332, -0.25336809,
		  -0.66667384, 1.61645574, 0.0157683,
		  0.01764248, -0.04277698, 0.94224328
	  } },

	/* Source: Colour python library. */
	/** ACES Primaries #0 (AP0). */
	{ .sz_rows = 3,
	  .sz_cols = 3,
	  .data = (zsl_real_t[]) {
		  1.04981102e+00, 0.00000000e+00, -9.74845410e-05,
		  -4.95903023e-01, 1.37331305e+00, 9.82400365e-02,
		  0.00000000e+00, 0.00000000e+00, 9.91252022e-01
	  } },

	/* Source: Colour python library. */
	/** ACES Primaries #1 (AP1). */
	{ .sz_rows = 3,
	  .sz_cols = 3,
	  .data = (zsl_real_t[]) {
		  1.64102338, -0.32480329, -0.2364247,
		  -0.66366286, 1.61533159, 0.01675635,
		  0.01172189, -0.00828444, 0.98839486
	  } },

	/* Source: Colour python library. */
	/** DCI-P3. */
	{ .sz_rows = 3,
	  .sz_cols = 3,
	  .data = (zsl_real_t[]) {
		  2.72539403, -1.01800301, -0.4401632,
		  -0.79516803, 1.68973205, 0.02264719,
		  0.04124189, -0.08763902, 1.10092938
	  } },

	/* Source: Colour python library. */
	/** DCI-P3+. */
	{ .sz_rows = 3,
	  .sz_cols = 3,
	  .data = (zsl_real_t[]) {
		  1.99040349, -0.56139586, -0.22966194,
		  -0.45849279, 1.262346, 0.15487549,
		  0.01563207, -0.00440904, 1.03772867
	  } },

	/* Source: Colour python library. */
	/** CIE linear RGB. */
	{ .sz_rows = 3,
	  .sz_cols = 3,
	  .data = (zsl_real_t[]) {
		  2.37067401, -0.9000403, -0.47063371,
		  -0.51388479, 1.42530348, 0.0885813,
		  0.00529816, -0.0146949, 1.00939674
	  } },
};

void
zsl_clr_rgbccm_get(enum zsl_clr_rgb_ccm ccm, struct zsl_mtx **mtx)
{
	*mtx = &zsl_clr_rgb_ccm_list[ccm];
}
