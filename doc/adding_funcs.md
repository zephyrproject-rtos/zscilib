# Adding Functions to zscilib

This document details the steps required to add a new function to zscilib,
including documentation and unit tests to ensure reliable execution of the
function.

## Function Prototype

Start by creating the function prototype in an appropriate header file in the
`include/zsl` folder.

**Functions must start with `zsl_group_*`** where `_group_` is an abbreviation
(5 chars max, ideally 3) of the group name, which helps associated source
files with functions. For example: `zsl_mtx_get` where `mtx` indicates that
this function is part of the `matrices` module.

**Function prototypes must be fully documenting using doxygen syntax.**

Note the following points concering the doxygen example below:

- Doxygen comment blocks start with a double asterix (`/**`).
- All parameters must be documented with the `@param <name>` field and a short
  comment describing the parameter in question.
- The return value should be documented, **particularly any error codes that may
  be returned by this function**.

```c
/**
 * @brief Gets a single value from the specified row (i) and column (j).
 *
 * @param m     Pointer to the zsl_mtx to use.
 * @param i     The row number to read (0-based).
 * @param j     The column number to read (0-based).
 * @param x     Pointer to where the value should be stored.
 *
 * @return  0 if everything executed correctly, or -EINVAL on an out of
 *          bounds error.
 */
int zsl_mtx_get(struct zsl_mtx *m, size_t i, size_t j, zsl_real_t *x);
```

### Facultative: Creating new header files

If the function is logically part of a new organisation block, also create a
new header file with the following format:

> Replace GROUPNAME and related strings with appropriate values. See existing
  header files as a reference.

```c
/*
 * Copyright (c) 2019 Author Name Here
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * \defgroup GROUPNAME Group name
 *
 * @brief Group name functions.
 */

/**
 * @file
 * @brief API header file for group name in zscilib.
 *
 * This file contains the zscilib group name APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_GROUPNAME_H_
#define ZEPHYR_INCLUDE_ZSL_GROUPNAME_H_

#include <zsl/zsl.h>

#ifdef __cplusplus
extern "C" {
#endif

/* TODO: Insert structs, enums and function prototypes here. */

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_GROUPNAME_H_ */

/** @} */ /* End of group name group */
```

## Function Implementation

Once the function prototype has been defined, the function source code can be
added to the appropriate source module file.

When adding a new function to an existing source module, **be sure that the
function appears in the source module (the .c file) in the same order it
appears in the header file**.

Use the following example as a starting point for formatting, etc.:

```c
int
zsl_mtx_get(struct zsl_mtx *m, size_t i, size_t j, zsl_real_t *x)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
        if ((i >= m->sz_rows) || (j >= m->sz_cols)) {
                return -EINVAL;
        }
#endif

        *x = m->data[(i * m->sz_cols) + j];

        return 0;
}
```

### Create new source modules

If necessary to create a new logical source module, the following template
must be followed for the .c file, replacing `zsl/groupname.h` with a reference
to the appropriate header file mentioned earlier in this guide.

```c
/*
 * Copyright (c) 2019 Author Name Here
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <math.h>
#include <errno.h>
#include <kernel.h>
#include <zsl/zsl.h>
#include <zsl/groupname.h>

/* TODO: Insert function implementations, etc., here. */
```

## Unit Tests

Unit tests **MUST be added for all new functions**, and changes or additions
will not be merged into the codebase without appropriate unit test coverage.

### Adding a unit test reference

To add a new unit test, open `tests/src/main.c`, and add a function prototype
for the new unit test with an appropriate name:

```c
extern void test_matrix_get(void);
```

Then add a call using that prototype inside `void test_main(void)`:

> Note the trailing comma (`,`) at the end of the function call. This must
be present unless the new function is the last entry in the test function list.

```c
void test_main(void) {
        ...
        ztest_unit_test(test_matrix_get),
        ...
}
```

### Implementing the unit test

To implement the unit test, open (or create) the appropriate `groupname_tests.c`
file and add a function to the file matching the unit test created above in
`main.c`:

```c
void test_matrix_get(void)
{
        int rc;
        zsl_real_t x;
        zsl_real_t data[9] = { 1.0, 0.0, 0.0,
                               0.0, 0.5, 0.0,
                               0.0, 0.0, 0.1 };

        struct zsl_mtx m = {
                .sz_rows = 3,
                .sz_cols = 3,
                .data = data
        };

        /* Read values from the matrix above. */
        rc = zsl_mtx_get(&m, 0, 0, &x);
        zassert_equal(rc, 0, NULL);
        zassert_true(val_is_equal(x, 1.0, 1E-5), NULL);

        rc = zsl_mtx_get(&m, 1, 0, &x);
        zassert_equal(rc, 0, NULL);
        zassert_equal(x, 0.0, NULL);

        rc = zsl_mtx_get(&m, 2, 2, &x);
        zassert_equal(rc, 0, NULL);
        zassert_true(val_is_equal(x, 0.1, 1E-5), NULL);

        /* Check for out of bounbds error. */
        zassert_true(zsl_mtx_get(&m, 3, 3, &x) == -EINVAL, NULL);
}
```

`zassert_` functions that can be used are documented as part of the
[Zephyr Test Framework](https://docs.zephyrproject.org/latest/guides/test/ztest.html).

See specifically the [defines](https://docs.zephyrproject.org/latest/guides/test/ztest.html#c.zassert) section of the documentation.

### Running the unit tests

At this point, you can run the unit tests for the new function as follows:

> This assumes you have already run **`$ source zephyr/zephyr-env.sh`**

```bash
$ twister -p qemu_cortex_m3 -T modules/lib/zscilib/tests
```

#### Resolving build/test failures

If you encounter any errors during the test run, they will generally be one of
the following two types:

- **build errors**, indicating a problem compiling the source code in your
  new function, indicated via the following warning:

```
qemu_cortex_m3    zsl.core.thumb2.double        FAILED: handler_crash
        see: twister-out/qemu_cortex_m3/zsl.core.thumb2.double/build.log
```

- **handler errors**, indicating that one of the test checks in your unit test
  code has failed during execution, which may be a problem with the function
  or an error in the unit test implemenation itself:

```
qemu_cortex_m3    zsl.core.thumb2.double        FAILED: failed
        see: twister-out/qemu_cortex_m3/zsl.core.thumb2.double/handler.log
```

To find the exact source of the error, simply examine the appropriate `.log`
file mentionned in the error message (`$ cat filename.log`,
`$ cat filename.log` etc.)
