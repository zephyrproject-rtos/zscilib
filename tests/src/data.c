/*
 * Copyright (c) 2019-2020 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>
#include <zsl/colorimetry.h>

/**
 * Spectral power distribution (aka spectral density) array based on natural
 * daylight at 5983K, 0.0033 Duv and 288 lux sampled on a spectrometer.
 *
 * 5983.00 K and 0.0033 Duv as CIE 1931 XYZ tristimulus =
 *
 *   o X: 0.9516492
 *   o Y: 1.0000000
 *   o Z: 1.0056107
 */
const struct zsl_clr_spd zsl_clr_test_spd_5983k = {
	.size = 21,
	.comps = {
		{ .nm = 380, .value = 0.290 },
		{ .nm = 400, .value = 0.471 },
		{ .nm = 420, .value = 0.719 },
		{ .nm = 440, .value = 0.826 },
		{ .nm = 460, .value = 1.006 },
		{ .nm = 480, .value = 0.989 },
		{ .nm = 500, .value = 0.955 },
		{ .nm = 520, .value = 0.910 },
		{ .nm = 540, .value = 0.950 },
		{ .nm = 560, .value = 0.912 },
		{ .nm = 580, .value = 0.848 },
		{ .nm = 600, .value = 0.814 },
		{ .nm = 620, .value = 0.827 },
		{ .nm = 640, .value = 0.771 },
		{ .nm = 660, .value = 0.749 },
		{ .nm = 680, .value = 0.766 },
		{ .nm = 700, .value = 0.705 },
		{ .nm = 720, .value = 0.599 },
		{ .nm = 740, .value = 0.850 },
		{ .nm = 760, .value = 0.699 },
		{ .nm = 780, .value = 0.898 }
	}
};
