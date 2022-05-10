/*
 * Copyright (c) 2021 Kevin Townsend
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>
#include <zsl/zsl.h>
#include <zsl/orientation/orientation.h>
#include <zsl/instrumentation.h>

static const struct device *amtdev = DEVICE_DT_GET_ANY(nxp_fxos8700);
static const struct device *gdev = DEVICE_DT_GET_ANY(nxp_fxas21002);

void work_handler(struct k_work *work)
{
	struct sensor_value accel[3];
	struct sensor_value mag[3];
	struct sensor_value gyro[3];
	struct sensor_value die_temp;

	/* Read data from the sensors. */
	sensor_sample_fetch(amtdev);
	sensor_sample_fetch(gdev);
	sensor_channel_get(amtdev, SENSOR_CHAN_ACCEL_XYZ, accel);
	sensor_channel_get(amtdev, SENSOR_CHAN_MAGN_XYZ, mag);
	sensor_channel_get(amtdev, SENSOR_CHAN_DIE_TEMP, &die_temp);
	sensor_channel_get(gdev, SENSOR_CHAN_GYRO_XYZ, gyro);

	/* Print timestamp in ns. */
	printf("%d, ", k_uptime_get_32());

	/* Print accel data. */
	printf("%5.3f, %5.3f, %5.3f, ",
	       sensor_value_to_double(&accel[0]),
	       sensor_value_to_double(&accel[1]),
	       sensor_value_to_double(&accel[2]));

	/* Print mag data. */
	printf("%5.3f, %5.3f, %5.3f, ",
	       sensor_value_to_double(&mag[0]),
	       sensor_value_to_double(&mag[1]),
	       sensor_value_to_double(&mag[2]));

	/* Print gyro data. */
	printf("%5.3f, %5.3f, %5.3f, ",
	       sensor_value_to_double(&gyro[0]),
	       sensor_value_to_double(&gyro[1]),
	       sensor_value_to_double(&gyro[2]));

	/* Print temperature. */
	printf("%3.2f", sensor_value_to_double(&die_temp));

	printf("\n");
}

K_WORK_DEFINE(work, work_handler);

void sensor_timer_handler(struct k_timer *dummy)
{
	k_work_submit(&work);
}

K_TIMER_DEFINE(sensor_timer, sensor_timer_handler, NULL);

void main(void)
{
	/* Make sure the accel+mag was found and properly initialised. */
	if (amtdev == NULL || !device_is_ready(amtdev)) {
		printf("Could not get FXOS8700 device\n");
		return;
	}

	/* Make sure the gyro was found and properly initialised. */
	if (gdev == NULL || !device_is_ready(gdev)) {
		printf("Could not get FXAS21002 device\n");
		return;
	}

	printf("ns, ax, ay, az, mx, my, mz, gz, gy, gz, temp_c\n");

	/* Start sensor timer (20 ms / 50 Hz) */
	k_timer_start(&sensor_timer, K_MSEC(10), K_MSEC(10));

	while (1) {
		k_sleep(K_MSEC(100));
	}
}
