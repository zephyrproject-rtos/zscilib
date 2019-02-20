/**
 * Depending on the type of package, there are different
 * compilation rules for this directory.  This comment applies
 * to packages of type "pkg."  For other types of packages,
 * please view the documentation at http://mynewt.apache.org/.
 *
 * Put source files in this directory.  All files that have a *.c
 * ending are recursively compiled in the src/ directory and its
 * descendants.  The exception here is the arch/ directory, which
 * is ignored in the default compilation.
 *
 * The arch/<your-arch>/ directories are manually added and
 * recursively compiled for all files that end with either *.c
 * or *.a.  Any directories in arch/ that don't match the
 * architecture being compiled are not compiled.
 *
 * Architecture is set by the BSP/MCU combination.
 */

#include <ctype.h>
#include "floatcheck.h"

bool
val_is_equal(zsl_data_t a, zsl_data_t b, zsl_data_t epsilon)
{
    zsl_data_t c;

    c = a - b;

    if (c < epsilon && -c < epsilon) {
        return 1;
    } else {
        return 0;
    }
}

bool
val_is_at_least(zsl_data_t a, zsl_data_t b)
{
    return a >= b ? 1 : 0;
}

bool
val_is_less_than(zsl_data_t a, zsl_data_t b)
{
    return a < b ? 1 : 0;
}

bool
val_is_greater_than(zsl_data_t a, zsl_data_t b)
{
    return a > b ? 1 : 0;
}

bool
val_is_within(zsl_data_t a, zsl_data_t upper, zsl_data_t lower)
{
    if ((a <= upper) && (a >= lower)) {
        return 1;
    } else {
        return 0;
    }
}
