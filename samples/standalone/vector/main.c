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

int main(void)
{	
	FILE * fh;
	size_t count = 0;
    fh = fopen("data2.txt","r");
	zsl_real_t d1, d2, d3, d4, d5, d6, d7, d8, d9, d10;

	while ((d1 = fgetc(fh)) != EOF) {
    	fscanf (fh, "%lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf",
	    &d1, &d2, &d3, &d4, &d5, &d6, &d7, &d8, &d9, &d10);
    	
		// printf ("\t%lf, %lf, %lf,\n", d2, d3, d4);
		printf ("\t%lf, %lf, %lf,\n", d5, d6, d7);
		// printf ("\t%lf, %lf, %lf,\n", d8, d9, d10);
		count++;
		// printf ("%lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf\n",
		// d1, d2, d3, d4, d5, d6, d7, d8, d9, d10);
  	}
	
	printf("\n%zu\n", count);

	return 0;
}
