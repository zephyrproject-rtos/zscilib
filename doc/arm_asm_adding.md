# Adding ARM ASM Functions to zscilib

In order to add [inline ARM assembly](arm_asm_gnu_inline.md) implementations
of existing C functions in zscilib, the steps described below should be
carefully followed.

These mechanisms ensure that the codebase maintains the flexibility to work
with other non-ARM architectures, and that the library safely falls back to C
function implementations when the conditions for the use of ARM Thumb assembly
are not met.

## TODO
