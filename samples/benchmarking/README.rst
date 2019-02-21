.. _zscilib-benchmark-sample:

zscilib Benchmark Test
######################

Overview
********

TODO

Requirements
************

The demo assumes that an LED is connected to one of GPIO lines. The
sample code is configured to work on boards that have defined the led0
alias in their board device tree description file. Doing so will generate
these variables:

- LED0_GPIO_CONTROLLER
- LED0_GPIO_PIN


Building and Running
********************

This samples does not output anything to the console.  It can be built and
flashed to a board as follows:

    .. code-block:: console

        $ cd samples/benchmarking
        $ mkdir b && cd b
        $ cmake -GNinja -DBOARD=qemu_cortex_m3 ..
        $ ninja

After flashing the image to the board, the user LED on the board should start to
blink.
