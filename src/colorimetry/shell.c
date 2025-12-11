/*
 * Copyright (c) 2019-2021 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdlib.h>
#include <ctype.h>
#include <zephyr/shell/shell.h>
#include <zsl/colorimetry.h>

#if CONFIG_ZSL_SHELL_CLR

static int
zsl_clr_shell_invalid_arg(const struct shell *shell, char *arg_name)
{
	shell_print(shell, "Error: invalid argument \"%s\"\n", arg_name);

	return -EINVAL;
}

static int
zsl_clr_shell_cmd_ct2rgb(const struct shell *shell, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	zsl_real_t ct = 0.0;
	struct zsl_clr_rgb8 rgb;
	struct zsl_mtx *srgb_ccm;

	/* sRGB D65 correlation matrix. */
	zsl_clr_rgbccm_get(ZSL_CLR_RGB_CCM_SRGB_D65, &srgb_ccm);

	memset(&rgb, 0, sizeof rgb);

	if ((strcmp(argv[1], "help") == 0)) {
		shell_print(shell, "Color temperature to sRGB (D65)\n");
		shell_print(shell, "$ %s %s 5600.0", argv[-1], argv[0]);
		shell_print(shell, "5600 K to sRGB (D65) = 255 248 224");
		shell_print(shell, "Gamut Warning: R..\n");
		shell_print(shell, "'R..' indicates red is out of gamut.");
		return 0;
	}

	/* Invalid color temperature. */
	ct = strtof(argv[1], NULL);
	if (ct <= 0.0) {
		return zsl_clr_shell_invalid_arg(shell, argv[1]);
	}

	/* Convert ct to sRGB and display 8-bit component values. */
	zsl_clr_conv_ct_rgb8(ct, ZSL_CLR_OBS_2_DEG, srgb_ccm, &rgb);
	shell_print(shell, "%d K to sRGB (D65) = %d %d %d",
		    (unsigned int)ct, rgb.r, rgb.g, rgb.b);

	/* Display an out of gamut warning. */
	if ((rgb.r_invalid) || (rgb.g_invalid) || (rgb.b_invalid)) {
		shell_print(shell, "Gamut Warning: %c%c%c",
			    rgb.r_invalid ? 'R' : '.',
			    rgb.g_invalid ? 'G' : '.',
			    rgb.b_invalid ? 'B' : '.');
	}

	return 0;
}

static int
zsl_clr_shell_cmd_ct2rgbf(const struct shell *shell, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	zsl_real_t ct = 0.0;
	struct zsl_clr_rgbf rgb;
	struct zsl_mtx *srgb_ccm;

	/* sRGB D65 correlation matrix. */
	zsl_clr_rgbccm_get(ZSL_CLR_RGB_CCM_SRGB_D65, &srgb_ccm);

	memset(&rgb, 0, sizeof rgb);

	if ((strcmp(argv[1], "help") == 0)) {
		shell_print(shell, "Color temperature to sRGB float (D65)\n");
		shell_print(shell, "$ %s %s 5600.0", argv[-1], argv[0]);
		shell_print(shell, "5600 K to sRGB (D65) = TODOO");
		shell_print(shell, "Gamut Warning: R..\n");
		shell_print(shell, "'R..' indicates red is out of gamut.");
		return 0;
	}

	/* Invalid color temperature. */
	ct = strtof(argv[1], NULL);
	if (ct <= 0.0) {
		return zsl_clr_shell_invalid_arg(shell, argv[1]);
	}

	/* Convert ct to sRGB and display floating point component values. */
	zsl_clr_conv_ct_rgbf(ct, ZSL_CLR_OBS_2_DEG, srgb_ccm, &rgb);
	shell_print(shell, "%d K to sRGB (D65) = %f %f %f\n",
		    (unsigned int)ct, rgb.r, rgb.g, rgb.b);

	/* Display an out of gamut warning. */
	if ((rgb.r_invalid) || (rgb.g_invalid) || (rgb.b_invalid)) {
		shell_print(shell, "Gamut Warning: %c%c%c",
			    rgb.r_invalid ? 'R' : '.',
			    rgb.g_invalid ? 'G' : '.',
			    rgb.b_invalid ? 'B' : '.');
	}

	return 0;
}

/* Subcommand array for "color" (level 1). */
SHELL_STATIC_SUBCMD_SET_CREATE(
	sub_color,
	/* 'rgb' command handler. */
	SHELL_CMD_ARG(ct2rgb, NULL, "ct to sRGB (D65)",
		      zsl_clr_shell_cmd_ct2rgb, 2, 0),
	/* 'rgbf' command handler. */
	SHELL_CMD_ARG(ct2rgbf, NULL, "ct to sRGB float (D65)",
		      zsl_clr_shell_cmd_ct2rgbf, 2, 0),

	/* Array terminator. */
	SHELL_SUBCMD_SET_END
	);

/* Root command "color" (level 0). */
SHELL_CMD_REGISTER(color, &sub_color, "Colorimetry commands", NULL);

#endif
