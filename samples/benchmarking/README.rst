.. _zscilib-benchmark-sample:

Benchmark Test
##############

Overview
********

This sample performs a set of calculations repetitively, tracking the time it
takes to execute each set of functions and outputting them in a standard
format to the console.

It can be used to determine the effect different compiler or device settings
have on code execution on the same or different platforms.

Accuracy
********

When running the benchmarking sample app on an actual ARM Cortex-M3 or M4
core with the **DWT** peripheral block present, you will get reasonably
accurate benchmark data.

Using ``qemu_cortex_m3`` or non M3/M4 hardware, the high-precision kernel clock
is used to track execution time, so the benchmark data is approximate and not
comparable across devices.

Requirements
************

Depending on the device you are testing with, you may need to enable or
disable the ``CONFIG_STDOUT_CONSOLE`` flag.

It should be set to ``y`` when using the emulator, and to ``n`` when running on
real hardware.

Building and Running
********************

To run the benchmarks on the **Cortex M3 emulator**, run the following commands
which will compile the application, run it on the emulator, and output
the result to the console:

.. code-block:: console

    $ west build -b qemu_cortex_m3 samples/benchmarking/ -t run

To run the application on real HW, typically outputting the results to the
serial port, you can try a variant of the following, adjusting ``-b``
as appropriate.

The **nRF52840dk** from Nordic Semiconductors is used below:

.. code-block:: console

    $ west build -b nrf52840dk/nrf52840 samples/benchmarking/
    $ west flash

Sample Output
*************

The benchmark application will normally output text resembling the following:

.. code-block:: console

    ***** Booting Zephyr OS zephyr-v1.14.0-39-ge03905354671 *****
    zscilib benchmark

    BOARD:                       nrf52840dk
    ZSL VERSION:                 0.1.0-prerelease (Apr 29 2019)
    CONFIG_ZSL_PLATFORM_OPT:     2
    CONFIG_ZSL_SINGLE_PRECISION: False
    CONFIG_ZSL_VECTOR_INLINE:    False
    CONFIG_ZSL_MATRIX_INLINE:    False
    CONFIG_ZSL_BOUNDS_CHECKS:    True

    zsl_vec_add : 333 cycles
    zsl_vec_add : 339 cycles
    zsl_vec_add : 339 cycles
