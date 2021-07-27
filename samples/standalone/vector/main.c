/*
 * Copyright (c) 2019-2020 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "zsl/matrices.h"
#include "zsl/vectors.h"
#include "zsl/statistics.h"
#include "zsl/probability.h"
#include "zsl/orientation/orientation.h"

int
main(void)
{
	struct zsl_quat qin = { .r = 1.0, .i = 0.0, .j = 0.0, .k = 0.0 };
	struct zsl_quat qout;
	ZSL_VECTOR_DEF(w, 3);
	zsl_real_t a[3] = { 1.5 * 3.0, 4.4 * 3.0, -12.8 * 3.0 };
	zsl_real_t t = 0.01, i = 3.0;

	zsl_vec_from_arr(&w, a);

	zsl_quat_from_ang_mom(&w, &qin, &i, &t, &qout);

	zsl_quat_print(&qout);

	return 0;
}
