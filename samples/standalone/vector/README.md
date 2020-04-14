# Standalone zscilib example (non-Zephyr)

This sample shows how zscilib can be used in a stand-alone (non Zephyr-based)
projects using a standard makefile (`Makefile`).

`gcc` is used by default as the target compiler, but the exact compiler version
can be easily changed in the Makefile.

## Functionality

- Adds two vector together.

## Enabling Optimised Functions

You can enable the optimised ARM THUMB-2 (etc.) assembly versions of certain
vector functions by setting the following config flag in the `Makefile`:

```
# Optionally enable ARM THUMB-2 ASM optimised functions
CFLAGS += -DCONFIG_ZSL_PLATFORM_OPT=2
```

This will cause the ASM functions in `include/zsl/asm/arm/asm_arm_vectors.h`
to be used in place of the standard C function code.

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
gcc  obj/main.o obj/matrices.o obj/vectors.o obj/zsl.o  -o bin/zscilib -Wall -Wconversion -Wno-sign-conversion -I. -I../../../include -lm
```

You can then run the resulting binary as follows:

```bash
bin/zscilib
```

Which should give you the following partial results:
```
Hello, zscilib!
-1.000000 1.500000 2.100000
```
