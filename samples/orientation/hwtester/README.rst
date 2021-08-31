.. _zscilib-orientation-hw-sample:

Orientation Hardware Test
#########################

Overview
********

This sample allows testing the orientation API with real hardware, using
Zephyr's sensor API to access live sensor data.

It has been tested with the ``frdm_k64f`` target using an external
**FXOS8700CB** 3-axis accelerometer and magnetometer, and an **FXAS21002C**
3-axis gyroscope. See the DTS overlay file for I2C addresses used, or to modify
them to use other values depending on the sensor breakout board(s) being used.

Requirements
************

If you are using a different board and sensor combination than the
``frdm_k64f`` and an external FXOS8700CB and FXAS21002C, you will
need to add a custom board config and device tree overlay to the ``boards``
folder. Follow the existing example for the frdm_k64f.

Building and Running
********************

To run the sample on the ``frdm_k64f``, run the following commands:

.. code-block:: console

    $ west build -p auto -b frdm_k64f samples/orientation/hwtester
    $ west flash

Then open up the serial port, using a tool like ``minicom``:

.. code-block:: console

   $ minicom -D /dev/tty.usbmodem14302

To quit minicom, press ESC+x

Sample Output
*************

The sample will output the following text:

.. code-block:: console

   ***** Booting Zephyr OS zephyr-v1.14.0-39-ge03905354671 *****
   ns, ax, ay, az, mx, my, mz, gz, gy, gz, temp_c
   161, 0.819, 0.716, 9.634, 0.000, 0.000, 0.000, 0.042, 0.086, 0.008, 23.04
   171, 0.819, 0.716, 9.634, 0.000, 0.000, 0.000, 0.043, 0.087, 0.008, 23.04
