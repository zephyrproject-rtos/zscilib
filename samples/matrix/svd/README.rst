.. _zscilib-svd-sample:

Benchmark Test
##############

Overview
********

TODO

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

    $ cd samples/benchmarking
    $ mkdir b && cd b
    $ cmake -GNinja -DBOARD=qemu_cortex_m3 ..
    $ ninja
    $ ninja run

To run the application on real HW, typically outputting the results to the
serial port, you can try a variant of the following, adjusting ``-DBOARD``
as appropriate.

The **nRF52840 PCA10056** from Nordic Semiconductors is used below:

.. code-block:: console

    $ cd samples/benchmarking
    $ mkdir b && cd b
    $ cmake -GNinja -DBOARD=nrf52840_pca10056 ..
    $ ninja
    $ ninja flash

Sample Output
*************

The benchmark application will normally output text resembling the following:

.. code-block:: console

    TODO
