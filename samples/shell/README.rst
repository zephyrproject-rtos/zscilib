.. _zclr-shell-sample:

zscilib Shell Example
#####################

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

To run this example on the **Cortex M3 emulator**, run the following commands
which will compile the application, run it on the emulator, and output
the result to the console:

.. code-block:: console

    $ cd samples/shell
    $ mkdir b && cd b
    $ cmake -GNinja -DBOARD=qemu_cortex_m3 ..
    $ ninja
    $ ninja run

To do the same thing using ``west``, run:

.. code-block:: console

   $ west build -p -b qemu_cortex_m3 samples/shell/ -t run

Press ``CTRL+A`` to exit QEMU.

To run the application on real HW, typically outputting the results to the
serial port, you can try a variant of the following, adjusting ``-DBOARD``
as appropriate.

The **nRF52840dk** from Nordic Semiconductors is used below:

.. code-block:: console

    $ cd samples/shell
    $ mkdir b && cd b
    $ cmake -GNinja -DBOARD=nrf52840dk/nrf52840 ..
    $ ninja
    $ ninja flash

To do the same thing using ``west`, run:

.. code-block:: console

   $ west build -p -b nrf52840dk/nrf52840 samples/shell/
