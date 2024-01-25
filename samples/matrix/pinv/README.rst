.. _zscilib-mtx-pinv-sample:

Pseudo-inverse (pinv)
#####################

Overview
********

This sample application shows how to calculate the pseudo-inverse of
an m x n matrix using zscilib. The results will be sent to the console for
verification.

Requirements
************

Depending on the device you are testing with, you may need to enable or
disable the ``CONFIG_STDOUT_CONSOLE`` flag.

It should be set to ``y`` when using the emulator, and to ``n`` when running on
real hardware.

Building and Running
********************

To run this sample on the **Cortex M3 emulator**, run the following commands
which will compile the application, run it on the emulator, and output
the result to the console:

.. code-block:: console

    $ west build -p -b mps2/an521/cpu0 samples/matrix/pinv -t run

To run the application on real HW, typically outputting the results to the
serial port, you can try a variant of the following, adjusting ``-b BOARD``
as appropriate. The **nRF52840dk** from Nordic Semiconductors is used
below:

.. code-block:: console

    $ west build -p -b nrf52840/nrf52840dk samples/matrix/pinv
    $ west flash

Debugging
*********

You can debug the application with GDB on actual hardware if you have a
supported debugger and development board.

To debug your program on the **nRF52840dk**, for example, run:

.. code-block:: console

  $ west build -p -b nrf52840/nrf52840dk samples/matrix/pinv
  $ west flash
  $ west debug --runner jlink

To set a breakpoint, reset the program, and continue running:

.. code-block:: console

  # Set a breakpoint at main in main.c
  (gdb) break main.c:main
  Breakpoint 1 at 0xFFFF: ... main.c, line xx.

  # Reset the device
  (gdb) monitor reset
  Resetting target

  # Continue program execution
  (gdb) c
  Continuing.
  Breakpoint 1, main () at .../main.c:69
  69      {

Some useful GDB commands to research are:

.. code-block:: console

  break [filename:line/function]
  info variables | locals | args | stack | break | registers
  continue (c)
  step
  next (n)
  list
  print [var]
  bt
