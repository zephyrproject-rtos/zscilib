/*
 * Copyright (c) 2021 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdlib.h>
#include <ctype.h>
#include <zephyr/shell/shell.h>
#include <zsl/orientation/orientation.h>

#if CONFIG_ZSL_SHELL_ORIENT

static int
zsl_orient_shell_invalid_arg(const struct shell *shell, char *arg_name)
{
	shell_print(shell, "Error: invalid argument \"%s\"\n", arg_name);

	return -EINVAL;
}

static int
zsl_orient_shell_missing_arg(const struct shell *shell, char *arg_name)
{
	shell_print(shell, "Error: missing argument: \"%s\"\n", arg_name);

	return -EINVAL;
}

static int
zsl_orient_shell_rc_code(const struct shell *shell, int rc)
{
	shell_print(shell, "Error: unexpected return code: \"%d\"\n", rc);

	return -EINVAL;
}

static int
zsl_orient_shell_cmd_lat2g(const struct shell *shell, size_t argc, char **argv)
{
	int rc;
	zsl_real_t lat = 0.0;
	zsl_real_t alt = 0.0;
	zsl_real_t g = 0.0;

	if ((argc == 1) || (strcmp(argv[1], "help") == 0)) {
		shell_print(shell, "Latitude and altitude to gravity (m/s^2)");
		shell_print(shell, "  $ %s <lat> <altitude>\n", argv[0]);
		shell_print(shell, "Ex. (Barcelona, ES):");
		shell_print(shell, "  $ %s 41.3851 12.0", argv[0]);
		shell_print(shell, "  9.794146 m/s^2");
		return 0;
	}

	/* Missing argument. */
	if (argc == 2) {
		return zsl_orient_shell_missing_arg(shell, "altitude");
	}

	/* Too many arguments. */
	if (argc > 3) {
		return zsl_orient_shell_invalid_arg(shell, argv[3]);
	}

	/* Invalid latitude. */
	lat = strtof(argv[1], NULL);
	if (lat <= 0.0) {
		return zsl_orient_shell_invalid_arg(shell, argv[1]);
	}

	/* Invalid altitude. Use sensible Earth limits for validation. */
	alt = strtof(argv[2], NULL);
	if ((alt > 10000.0) || (alt < -500.0)) {
		return zsl_orient_shell_invalid_arg(shell, argv[2]);
	}

	rc = zsl_grav_lat_alt(lat, alt, &g);
	if (rc) {
		return zsl_orient_shell_rc_code(shell, rc);
	}

	shell_print(shell, "%f m/s^2", g);

	return 0;
}

/* Subcommand array for "orient" (level 1). */
SHELL_STATIC_SUBCMD_SET_CREATE(
	sub_orient,
	/* 'lat2g' command handler. */
	SHELL_CMD_ARG(lat2g, NULL, "Latitude and altitude to gravity",
		      zsl_orient_shell_cmd_lat2g, 1, 4),

	/* Array terminator. */
	SHELL_SUBCMD_SET_END
	);

/* Root command "orient" (level 0). */
SHELL_CMD_REGISTER(orient, &sub_orient, "Orientation commands", NULL);

#endif
