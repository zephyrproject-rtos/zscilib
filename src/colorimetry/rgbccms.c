/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <errno.h>
#include <zsl/colorimetry.h>

/**
 * @brief 3x3 XYZ to RGB correlation matrices.
 */
static struct zsl_mtx zsl_clr_rgb_ccm_list[] = {
	/** sRGB D65. */
	{ .sz_rows = 3,
	  .sz_cols = 3,
	  .data = (zsl_real_t[]) {
		  3.2404542, -1.5371385, -0.4985314,
		  -0.9692660, 1.8760108,  0.0415560,
		  0.0556434, -0.2040259,  1.0572252
	  } },
	/** sRGB D50. */
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

};

void
zsl_clr_rgbccm_get(enum zsl_clr_rgb_ccm ccm, struct zsl_mtx **mtx)
{
	*mtx = &zsl_clr_rgb_ccm_list[ccm];
}
