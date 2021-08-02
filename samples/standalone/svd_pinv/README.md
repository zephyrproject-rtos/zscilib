# Standalone zscilib example (non-Zephyr) 

This sample shows how zscilib can be used in a stand-alone (non Zephyr-based)
projects using a standard makefile (`Makefile`).

`gcc` is used by default as the target compiler, but the exact compiler version
can be easily changed in the Makefile.

## Functionality

- Calculates the **SVD** of an input matrix (`zsl_mtx_svd`), and verifying the
  results by recalculating the original input matrix from the U, Sigma, V^t
  output matrices where input = U * Sigma * V^t.
- Calculates the **Moore-Penrose pseudoinverse** (pinv) of an 18x3 input matrix
  (`zsl_mtx_pinv`), displaying the resulting 3x18 output.

## Using this Example

To build this example, simply run the following command(s):

```bash
make clean
make
```

Which should produce the following output:

```bash
Compiling obj/main.o
Compiling obj/matrices.o
Compiling obj/vectors.o
Compiling obj/zsl.o
Compiling obj/atomic.o
Compiling obj/dynamics.o
Compiling obj/eleccomp.o
Compiling obj/electric.o
Compiling obj/energy.o
Compiling obj/fluids.o
Compiling obj/gases.o
Compiling obj/gravitation.o
Compiling obj/kinematics.o
Compiling obj/magnetics.o
Compiling obj/mass.o
Compiling obj/misc.o
Compiling obj/momentum.o
Compiling obj/optics.o
Compiling obj/photons.o
Compiling obj/projectiles.o
Compiling obj/relativity.o
Compiling obj/rotation.o
Compiling obj/sound.o
Compiling obj/thermo.o
Compiling obj/waves.o
Compiling obj/work.o
gcc  obj/main.o obj/matrices.o obj/vectors.o obj/zsl.o obj/atomic.o obj/dynamics.o obj/eleccomp.o obj/electric.o obj/energy.o obj/fluids.o obj/gases.o obj/gravitation.o obj/kinematics.o obj/magnetics.o obj/mass.o obj/misc.o obj/momentum.o obj/optics.o obj/photons.o
obj/projectiles.o obj/relativity.o obj/rotation.o obj/sound.o obj/thermo.o obj/waves.o obj/work.o  -o bin/zscilib -Wall -Wconversion -Wno-sign-conversion -I. -I../../include -lm
```

You can then run the resulting binary as follows:

```bash
bin/zscilib
```

Which should give you the following partial results:
```
...
PSEUDOINVERSE TEST
------------------
INPUT (18x3 MATRIX)
-------------------
3.200000 5.000000 -4.200000
-3.100000 5.400000 -2.200000
7.700000 8.700000 9.900000
8.900000 0.600000 5.400000
3.700000 -3.300000 7.200000
-5.400000 -0.600000 8.400000
2.400000 5.100000 -5.700000
6.900000 -2.100000 0.400000
6.400000 -9.100000 -4.400000
0.100000 7.400000 0.000000
6.100000 -2.300000 5.500000
6.100000 -8.400000 -6.600000
7.100000 2.300000 4.100000
-0.800000 -4.700000 9.900000
-3.100000 1.200000 5.200000
6.400000 -6.300000 5.200000
8.800000 7.300000 4.200000
-4.700000 0.000000 -0.400000

PSEUDOINVERSE (3x18 MATRIX)
--------------------------
0.008305 -0.004161 0.011124 0.014273 0.003411 -0.013626 0.007470 0.012358 0.012618 0.001077 0.008695 0.013077 0.011694 -0.006234 -0.007796 0.008896 0.015403 -0.008536
0.011157 0.010817 0.016409 0.001246 -0.007502 -0.003439 0.011544 -0.003434 -0.016550 0.014775 -0.004896 -0.014773 0.004667 -0.011349 0.001035 -0.012783 0.014828 -0.000490
-0.009946 -0.003717 0.013118 0.005953 0.012168 0.017688 -0.012357 -0.001791 -0.008991 -0.001445 0.007869 -0.013002 0.004026 0.019251 0.010549 0.007954 0.002542 0.001259
```
