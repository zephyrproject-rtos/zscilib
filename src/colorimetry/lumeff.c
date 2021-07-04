/*
 * Copyright (c) 2019, 2020 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zsl/colorimetry.h>

/**
 * CIE 1988 Photopic Luminous Efficiency Function.
 * 380nm to 780nm in 5nm increments (81 values).
 */
static const struct zsl_clr_spd zsl_clr_conv_lef_cie88_photopic_5nm = {
	.size = 81,
	.comps =
	{
		{ .nm = 380, .value = 0.00020000 },
		{ .nm = 385, .value = 0.00039600 },
		{ .nm = 390, .value = 0.00080000 },
		{ .nm = 395, .value = 0.00155000 },
		{ .nm = 400, .value = 0.00280000 },
		{ .nm = 405, .value = 0.00466000 },
		{ .nm = 410, .value = 0.00740000 },
		{ .nm = 415, .value = 0.01180000 },
		{ .nm = 420, .value = 0.01750000 },
		{ .nm = 425, .value = 0.02270000 },
		{ .nm = 430, .value = 0.02730000 },
		{ .nm = 435, .value = 0.03260000 },
		{ .nm = 440, .value = 0.03790000 },
		{ .nm = 445, .value = 0.04240000 },
		{ .nm = 450, .value = 0.04680000 },
		{ .nm = 455, .value = 0.05210000 },
		{ .nm = 460, .value = 0.06000000 },
		{ .nm = 465, .value = 0.07390000 },
		{ .nm = 470, .value = 0.09100000 },
		{ .nm = 475, .value = 0.11300000 },
		{ .nm = 480, .value = 0.13900000 },
		{ .nm = 485, .value = 0.16900000 },
		{ .nm = 490, .value = 0.20800000 },
		{ .nm = 495, .value = 0.25900000 },
		{ .nm = 500, .value = 0.32300000 },
		{ .nm = 505, .value = 0.40730000 },
		{ .nm = 510, .value = 0.50300000 },
		{ .nm = 515, .value = 0.60820000 },
		{ .nm = 520, .value = 0.71000000 },
		{ .nm = 525, .value = 0.79320000 },
		{ .nm = 530, .value = 0.86200000 },
		{ .nm = 535, .value = 0.91485000 },
		{ .nm = 540, .value = 0.95400000 },
		{ .nm = 545, .value = 0.98030000 },
		{ .nm = 550, .value = 0.99495000 },
		{ .nm = 555, .value = 1.00000000 },
		{ .nm = 560, .value = 0.99500000 },
		{ .nm = 565, .value = 0.97860000 },
		{ .nm = 570, .value = 0.95200000 },
		{ .nm = 575, .value = 0.91540000 },
		{ .nm = 580, .value = 0.87000000 },
		{ .nm = 585, .value = 0.81630000 },
		{ .nm = 590, .value = 0.75700000 },
		{ .nm = 595, .value = 0.69490000 },
		{ .nm = 600, .value = 0.63100000 },
		{ .nm = 605, .value = 0.56680000 },
		{ .nm = 610, .value = 0.50300000 },
		{ .nm = 615, .value = 0.44120000 },
		{ .nm = 620, .value = 0.38100000 },
		{ .nm = 625, .value = 0.32000000 },
		{ .nm = 630, .value = 0.26500000 },
		{ .nm = 635, .value = 0.21700000 },
		{ .nm = 640, .value = 0.17500000 },
		{ .nm = 645, .value = 0.13820000 },
		{ .nm = 650, .value = 0.10700000 },
		{ .nm = 655, .value = 0.08160000 },
		{ .nm = 660, .value = 0.06100000 },
		{ .nm = 665, .value = 0.04460000 },
		{ .nm = 670, .value = 0.03200000 },
		{ .nm = 675, .value = 0.02320000 },
		{ .nm = 680, .value = 0.01700000 },
		{ .nm = 685, .value = 0.01190000 },
		{ .nm = 690, .value = 0.00821000 },
		{ .nm = 695, .value = 0.00572000 },
		{ .nm = 700, .value = 0.00410000 },
		{ .nm = 705, .value = 0.00293000 },
		{ .nm = 710, .value = 0.00209000 },
		{ .nm = 715, .value = 0.00148000 },
		{ .nm = 720, .value = 0.00105000 },
		{ .nm = 725, .value = 0.00074000 },
		{ .nm = 730, .value = 0.00052000 },
		{ .nm = 735, .value = 0.00036100 },
		{ .nm = 740, .value = 0.00024900 },
		{ .nm = 745, .value = 0.00017200 },
		{ .nm = 750, .value = 0.00012000 },
		{ .nm = 755, .value = 0.00008480 },
		{ .nm = 760, .value = 0.00006000 },
		{ .nm = 765, .value = 0.00004240 },
		{ .nm = 770, .value = 0.00003000 },
		{ .nm = 775, .value = 0.00002120 },
		{ .nm = 780, .value = 0.00001500 }
	}
};

/**
 * CIE 1951 Scotopic Luminous Efficiency Function.
 * 380nm to 780nm in 5nm increments (81 values).
 */
static const struct zsl_clr_spd zsl_clr_conv_lef_cie51_scotopic_5nm = {
	.size = 81,
	.comps =
	{
		{ .nm = 380, .value = 5.89E-01 },
		{ .nm = 385, .value = 1.108E+00 },
		{ .nm = 390, .value = 2.209E+00 },
		{ .nm = 395, .value = 4.53E+00 },
		{ .nm = 400, .value = 9.29E+00 },
		{ .nm = 405, .value = 1.852E+01 },
		{ .nm = 410, .value = 3.484E+01 },
		{ .nm = 415, .value = 6.04E+01 },
		{ .nm = 420, .value = 9.66E+01 },
		{ .nm = 425, .value = 1.436E+02 },
		{ .nm = 430, .value = 1.998E+02 },
		{ .nm = 435, .value = 2.625E+02 },
		{ .nm = 440, .value = 3.281E+02 },
		{ .nm = 445, .value = 3.931E+02 },
		{ .nm = 450, .value = 4.55E+02 },
		{ .nm = 455, .value = 5.13E+02 },
		{ .nm = 460, .value = 5.67E+02 },
		{ .nm = 465, .value = 6.2E+02 },
		{ .nm = 470, .value = 6.76E+02 },
		{ .nm = 475, .value = 7.34E+02 },
		{ .nm = 480, .value = 7.93E+02 },
		{ .nm = 485, .value = 8.51E+02 },
		{ .nm = 490, .value = 9.04E+02 },
		{ .nm = 495, .value = 9.49E+02 },
		{ .nm = 500, .value = 9.82E+02 },
		{ .nm = 505, .value = 9.98E+02 },
		{ .nm = 510, .value = 9.97E+02 },
		{ .nm = 515, .value = 9.75E+02 },
		{ .nm = 520, .value = 9.35E+02 },
		{ .nm = 525, .value = 8.8E+02 },
		{ .nm = 530, .value = 8.11E+02 },
		{ .nm = 535, .value = 7.33E+02 },
		{ .nm = 540, .value = 6.5E+02 },
		{ .nm = 545, .value = 5.64E+02 },
		{ .nm = 550, .value = 4.81E+02 },
		{ .nm = 555, .value = 4.02E+02 },
		{ .nm = 560, .value = 3.288E+02 },
		{ .nm = 565, .value = 2.639E+02 },
		{ .nm = 570, .value = 2.076E+02 },
		{ .nm = 575, .value = 1.602E+02 },
		{ .nm = 580, .value = 1.212E+02 },
		{ .nm = 585, .value = 8.99E+01 },
		{ .nm = 590, .value = 6.55E+01 },
		{ .nm = 595, .value = 4.69E+01 },
		{ .nm = 600, .value = 3.315E+01 },
		{ .nm = 605, .value = 2.312E+01 },
		{ .nm = 610, .value = 1.593E+01 },
		{ .nm = 615, .value = 1.088E+01 },
		{ .nm = 620, .value = 7.37E+00 },
		{ .nm = 625, .value = 4.97E+00 },
		{ .nm = 630, .value = 3.335E+00 },
		{ .nm = 635, .value = 2.235E+00 },
		{ .nm = 640, .value = 1.497E+00 },
		{ .nm = 645, .value = 1.005E+00 },
		{ .nm = 650, .value = 6.77E-01 },
		{ .nm = 655, .value = 4.59E-01 },
		{ .nm = 660, .value = 3.129E-01 },
		{ .nm = 665, .value = 2.146E-01 },
		{ .nm = 670, .value = 1.48E-01 },
		{ .nm = 675, .value = 1.026E-01 },
		{ .nm = 680, .value = 7.15E-02 },
		{ .nm = 685, .value = 5.01E-02 },
		{ .nm = 690, .value = 3.533E-02 },
		{ .nm = 695, .value = 2.501E-02 },
		{ .nm = 700, .value = 1.78E-02 },
		{ .nm = 705, .value = 1.273E-02 },
		{ .nm = 710, .value = 9.14E-03 },
		{ .nm = 715, .value = 6.6E-03 },
		{ .nm = 720, .value = 4.78E-03 },
		{ .nm = 725, .value = 3.482E-03 },
		{ .nm = 730, .value = 2.546E-03 },
		{ .nm = 735, .value = 1.87E-03 },
		{ .nm = 740, .value = 1.379E-03 },
		{ .nm = 745, .value = 1.022E-03 },
		{ .nm = 750, .value = 7.6E-04 },
		{ .nm = 755, .value = 5.67E-04 },
		{ .nm = 760, .value = 4.25E-04 },
		{ .nm = 765, .value = 3.196E-04 },
		{ .nm = 770, .value = 2.413E-04 },
		{ .nm = 775, .value = 1.829E-04 },
		{ .nm = 780, .value = 1.39E-04 }
	}
};

void
zsl_clr_lef_get(enum zsl_clr_lef lef, const struct zsl_clr_spd **data)
{
	switch (lef) {
	case ZSL_CLR_LEF_CIE51_SCOTOPIC:
		*data = &zsl_clr_conv_lef_cie51_scotopic_5nm;
		break;
	case ZSL_CLR_LEF_CIE88_PHOTOPIC:
	default:
		*data = &zsl_clr_conv_lef_cie88_photopic_5nm;
		break;
	}
}

int
zsl_clr_lef_lerp(enum zsl_clr_lef lef, unsigned int nm, zsl_real_t *val)
{
	const struct zsl_clr_spd *lef_data;
	size_t lower, upper;
	zsl_real_t coeff;

	/* Get a reference to the LEF data. */
	zsl_clr_lef_get(lef, &lef_data);

	/* Over/Underflow check. */
	if ((lef_data->comps[0].nm > nm) ||
	    (lef_data->comps[lef_data->size - 1].nm < nm)) {
		*val = 0.0;
		return 0;
	}

	/* Find the closest matching value. */
	upper = lower = 0;
	for (size_t i = 0; i < lef_data->size; i++) {
		/* Exact match */
		if (nm == lef_data->comps[i].nm) {
			*val = lef_data->comps[i].value;
			return 0;
		}
		/* Check if upper boundary found. */
		if (lef_data->comps[i].nm > nm) {
			upper = i;
			lower = i ? i - 1 : 0;
			break;
		}
	}

	/* Calculate the interpolation coefficient per nm interval.
	 * Since LEF data is in 5 nm steps, we can take some shortcuts here. */
	coeff = lef_data->comps[upper].value - lef_data->comps[lower].value;
	coeff /= 5.0;
	*val = coeff * (nm - lef_data->comps[lower].nm);
	*val += lef_data->comps[lower].value;

	return 0;
}
