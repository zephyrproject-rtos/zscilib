# TODO

This is incomplete, but identifies major short-term TODOs for zscilib.

## Matrices

### matrices.c: Missing boundary checks

Some functions require boundary checks, for example to check if the input
matrix is square or if the input index is not greater than the matrices
dimension.

### matrices.c: SVD and PINV

The sample applications for the svd and pseudoinverse functions still do not
work with large matrices, due to an excess memory usage. Memory optimisations
are required to use a pool of memory, and more efficient use of limited stack
resources.

### matrix_tests.c: Incomplete tests

The `test_matrix_unary_op`, `test_matrix_unary_func`, `test_matrix_binary_op`
and `test_matrix_binary_func` test functions are incomplete.

## Physics

### Sample apps

There are only a handful of sample applications for the physics groups.
Additional sample applications should be added.

### misc.c, relativity.c, waves.c

These groups of physics functiosn are currently incomplete.
