/*
 * Copyright (c) 2019-2021 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdlib.h>
#include <ctype.h>
#include <zephyr/shell/shell.h>
#include <zsl/zsl.h>

#if CONFIG_ZSL_SHELL

static int
zsl_shell_cmd_version(const struct shell *shell, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	shell_print(shell, "%s", zsl_version);

	return 0;
}

/* Subcommand array for "zsl" (level 1). */
SHELL_STATIC_SUBCMD_SET_CREATE(
	sub_zsl,
	/* 'version' command handler. */
	SHELL_CMD(version, NULL, "library version", zsl_shell_cmd_version),

	/* Array terminator. */
	SHELL_SUBCMD_SET_END
	);

/* Root command "zsl" (level 0). */
SHELL_CMD_REGISTER(zsl, &sub_zsl, "zscilib commands", NULL);

#endif
