# Zephyr Scientific Library Unit Tests

This folder contains unit tests for zsl.

## Running tests via `twister`

To run these units tests, make sure that you have previously run
`source zephyr-env.sh` (or it's equivalent for your system), and
execute the following command to start the test-runner:

```
$ twister --inline-logs -p mps2/an521/cpu0 -T [zscilib_tests_folder]
```

> **Tip**: If you wish to have **verbose output** to see any errors in
  the test output if you make changes, you can append the `-vv` flag.

## File structure

Unit tests are broken up into logical groups that correspond to the source
files in zsl.  When adding new tests, please try to respect the current
structure so that unit tests remain logically organised and easy to find.
