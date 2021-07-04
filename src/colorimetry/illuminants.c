/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <errno.h>
#include <zsl/colorimetry.h>

/**
 * @brief XYZ tristimulus values for CIE standard illuminants in the A, B, C,
 * D and E families, as well as the illuminant used in ICC profiles.
 */
static const struct zsl_clr_illum_data zsl_clr_illum_list[] = {
	{ .illuminant = ZSL_CLR_ILLUM_A,
	  .observer = ZSL_CLR_OBS_2_DEG,
	  .name = "A",
	  .data = {
		  .xyz_x = 1.0985,
		  .xyz_y = 1.00000,
		  .xyz_z = 0.3558
	  } },

	{ .illuminant = ZSL_CLR_ILLUM_B,
	  .observer = ZSL_CLR_OBS_2_DEG,
	  .name = "B",
	  .data = {
		  .xyz_x = 0.99093,
		  .xyz_y = 1.00000,
		  .xyz_z = 0.85313
	  } },

	{ .illuminant = ZSL_CLR_ILLUM_C,
	  .observer = ZSL_CLR_OBS_2_DEG,
	  .name = "C",
	  .data = {
		  .xyz_x = 0.9807,
		  .xyz_y = 1.00000,
		  .xyz_z = 1.1822
	  } },

	{ .illuminant = ZSL_CLR_ILLUM_D50,
	  .observer = ZSL_CLR_OBS_2_DEG,
	  .name = "D50",
	  .data = {
		  .xyz_x = 0.96422,
		  .xyz_y = 1.00000,
		  .xyz_z = 0.82521
	  } },

	{ .illuminant = ZSL_CLR_ILLUM_D55,
	  .observer = ZSL_CLR_OBS_2_DEG,
	  .name = "D55",
	  .data = {
		  .xyz_x = 0.9568,
		  .xyz_y = 1.00000,
		  .xyz_z = 0.9214
	  } },

	{ .illuminant = ZSL_CLR_ILLUM_D65,
	  .observer = ZSL_CLR_OBS_2_DEG,
	  .name = "D65",
	  .data = {
		  .xyz_x = 0.95047,
		  .xyz_y = 1.00000,
		  .xyz_z = 1.08883
	  } },

	{ .illuminant = ZSL_CLR_ILLUM_E,
	  .observer = ZSL_CLR_OBS_2_DEG,
	  .name = "E",
	  .data = {
		  .xyz_x = 1.00000,
		  .xyz_y = 1.00000,
		  .xyz_z = 1.00000
	  } },

	{ .illuminant = ZSL_CLR_ILLUM_ICC,
	  .observer = ZSL_CLR_OBS_2_DEG,
	  .name = "ICC",
	  .data = {
		  .xyz_x = 0.9642,
		  .xyz_y = 1.00000,
		  .xyz_z = 0.8249
	  } }
};

int
zsl_clr_illum_get(enum zsl_clr_obs obs, enum zsl_clr_illum illum,
	      const struct zsl_clr_illum_data **data)
{
	int count;

	count = sizeof(zsl_clr_illum_list) / sizeof(zsl_clr_illum_list[0]);

	/* Find supplied obs and illum. */
	for (size_t i = 0; i < count; i++) {
		if ((zsl_clr_illum_list[i].illuminant == illum) &&
		    (zsl_clr_illum_list[i].observer == obs)) {
			*data = &zsl_clr_illum_list[i];
			return 0;
		}
	}

	return -EINVAL;
}
