# Orientation Sensor Fusion

Sensor fusion algorithms are used to 'fuse' samples from several different
sensor types into a single output.

In the case of orientation sensors, this means fusing a combination of
accelerometer, magnetometer and/or gyroscope vectors into a quaternion output,
which can be converted to Euler angles or other units if desired.

## Algorithms

Most orientation fusion algorithms operate on similar principles: they take a
starting orientation, and integrate angular velocity from the gyroscope over a
known period of time, estimating a new orientation based on that integration.

The accelerometer and/or magnetometer vectors are used to help estimate
orientation in 3D space, relative to known values for gravity and/or magnetic
north. They are also used to help compensate for gyroscope drift over time,
which requires constant correction in MEMS gyroscopes.

The following fusion algorithms are supported by zscilib, with the stated
vector input requirements:

| Name          | Accel | Mag   | Gyro  | Note(s) |
|---------------|-------|-------|-------|---------|
| AQUA          | Opt.  | Opt.  | Yes   | 1       |
| Complementary | Opt.  | Opt.  | Yes   | 2       |
| Ext. Kalman   | Yes   | Yes   | Yes   |         |
| Madgwick      | Yes   | Opt.  | Yes   | 3       |
| Mahoney       | Yes   | Opt.  | Yes   | 3       |
| SAAM          | Yes   | Yes   | No    |         |

1. Magnetometer data can only be integrated in the presence of an accelerometer.
2. If accelerometer and magnetometer data are not both provided, only angular
   velocity will be used to estimate the change in orientation.
3. A magnetometer is optional, but is required for yaw/heading drift
   compensation, and should be included when possible.

### AQUA

AQUA, or "Algebraic Quaternion Algorithm", is a robust, well-designed algorithm
that starts by estimating orientation based solely on the angular velocity
(gyroscope), updating the starting orientation with an estimation of the new
orientation over time. In cases where only gyroscope data is provided, it acts
as a wrapper for `zsl_quat_from_ang_vel`.

If an accelerometer is also present, it will then refine the initial estimate,
taking into account the acceleration vector.

If a magnetometer AND an accelerometer are present, the magnetometer vector
will then be integrated into the results for the most robust orientation
estimation.

Note that when using this algorithm, if you have a gyroscope and a
magnetometer, but no accelerometer, the magnetometer data will be ignored
since integration of magnetometer data is built upon the missing accelerometer
integration stage.

Original Paper:

- https://res.mdpi.com/sensors/sensors-15-19302/article_deploy/sensors-15-19302.pdf

### Complementary

This algorithm starts by estimating orientation based solely on the angular
velocity (gyroscope), updating the starting orientation with an estimate of
the new position over time. In this case, it is simply a wrapper for
`zsl_quat_from_ang_vel`.

If accelerometer AND magnetometer data are also provided, a second distinct
orientation estimate will be made based on those two additional inputs
(`zsl_att_from_accelmag`). The two distinct orientation estimates (the gyro
quaternion and the accelmag quaternion) will be merged via linear interpolation
using a pre-determined 'weight' to produce the final output quaternion.

The weight of each estimate -- alpha -- is arbitrary and will vary, depending
on the reliability of the input sensors. If the gyroscope is considered more
reliable, alpha should be closer to zero. If the accel and mag are considered
more reliable, alpha should be closer to 1.0.

### Extended Kalman

The Extended Kalman Filter has traditionally been the go-to tool for
orientation fusion problems, since it is robust, stable and is very reliable
when properly configured.

Its a good choice for stable results, but has the unfortunate downside of being
computationally expensive, and may not be intuitive to understand for
non-specialists.

### Madgwick

This widely used, oft-quoted algorithm is computationally less expensive than
Extended Kalman filters, but implementations commonly contain subtle errors,
and default config values are often used without consideration of the actual
sensors being used, leading to inaccurate outputs.

To get valid results from this algorithm, it is imperative to determine the
correct `beta` value, which will vary depending on the physical sensor(s)
being used. When properly tuned, however, this algorithm can be stable over a
meaningful period of time, and allow faster throughput than Extended Kalman
Filters.

> While this algorithm can be used with only an accelerometer and a gyroscope,
  a magnetometer should always be provided to account for gyroscope drift
  around the vertical axis (yaw/heading).

Original Paper:

- https://www.x-io.co.uk/res/doc/madgwick_internal_report.pdf

### Mahoney

TODO

> While this algorithm can be used with only an accelerometer and a gyroscope,
  a magnetometer should always be provided to account for gyroscope drift
  around the vertical axis (yaw/heading).

Original Paper:

- https://hal.archives-ouvertes.fr/hal-00488376/document

### SAAM

SAAM, or "Super-fast Attitude from Accelerometer and Magnetometer", is a
highly computationally efficient algorithm based upon the "least-squares
optimisation", and only requires accelerometer and magnetometer vectors.

This algorithm is useful in extremely power or cost-sensitive applications,
since MEMS gyroscopes tend to have higher power consumption, and are more
expensive to manufacture than accelerometers and magnetometers.

Original Paper:

- https://hal.inria.fr/hal-01922922/document

## Calibration

All fusion algorithms require calibration to deliver meaningful results,
particularly with noisy, inexpensive MEMS sensors.

This is generally a two-step process:

### 1. Sensor Calibration

The input vectors (accel, mag, gyro) must be analysed in order to account for:

- **Hard-iron** and/or **soft-iron** errors or deformed responses in
  the magnetometer.
- **Gravitational offset(s)** in the accelerometer.
- **'Noise'** in the stationary sensor outputs.

This process is entirely independent of the fusion algorithm used, and all
sensor types will generally share the same calibration or characterisation
process.

The two most common calibration coefficients -- which must be obtained
empirically through testing -- are **bias/offset** error, and
**scale factor/sensitivity** error.

For linear sensors, taking into account these two variables, the calibrated
outputs can be obtained for each axis via:

```
output = scale_factor * input + offset
```

In the case of magnetometers, where the raw (generally ellipsoid) output values
need to be reshaped to fit inside a sphere, more complex calibration procedures
are usually required, such as **least squares fitting**, or specialised
algorithms like **minimum volume enclosing ellipsoid**.

zscilib provides functions to enable the derivation and application of these
specialised calibration parameters.

Depending on your requirements, you may also need to:

- Adjust magnetometer outputs to account for the difference between **magnetic
  north** and **geographic north**.
- Adjust accelerometer readings based on the gravitational constant at a
  **specific latitude and altitude**, since this varies according to position
  on Earth.

Helper functions for these requirements are provided as part of the general
orientation API, specifically `TBD` and `zsl_grav_lat_alt`.

### 2. Algorithm Configuration

The fusion algorithm must also be 'tuned' for the sensor input(s) it is being
fed. What this process entails depends on the algorithm used, but this
generally involves taking into account drift on the gyroscope, and setting a
tradeoff between fast integration time of changes, and reliable intermediary
results.

The implementation of each fusion algorithm should be consulted to understand
the config settings for that filter, and how to test for and adjust them for
the selected algorithm.

## Fusion Driver Interface

All orientation fusion algorithms in zscilib use the same interface, meaning
that it's trivial to switch between different algorithms and quickly evaluate
which algorithm provides the right tradeoffs between accuracy, precision,
and performance, based on the types of inputs you have available.

They implement an `init` function with the following signature:

```c
typedef int (*zsl_fus_init_cb_t)(uint32_t freq, void* cfg);
```

A `feed` function with the following signature:

```c
typedef int (*zsl_fus_feed_cb_t)(struct zsl_vec *accel,
             struct zsl_vec *mag, struct zsl_vec *gyro,
             struct zsl_quat *q, void* cfg);
```

And an `error` function with the following signature, which is called when an
error occurs in the `init` or `error` function implementations:

```c
typedef void (*zsl_fus_error_cb_t)(int error);
```

Each algorithm also defines a custom **config struct** that contains config
settings used to fine-tune the algorithm, which is assigned as part of the
`zsl_fus_drv` struct:

```c
struct zsl_fus_drv {
	zsl_fus_init_cb_t init_handler;
	zsl_fus_feed_cb_t feed_handler;
	zsl_fus_error_cb_t error_handler;
	void *config;
};
```

### Example

The Madgwick algorithm could be instantiated and configured via:

```c
static struct zsl_fus_madg_cfg madg_cfg = {
	.beta = 0.018,
};

static struct zsl_fus_drv madgwick_drv = {
	.init_handler = zsl_fus_madg_init,
	.feed_handler = zsl_fus_madg_feed,
	.error_handler = zsl_fus_madg_error,
	.config = &madg_cfg,
};
```

The init and feed functions can then be called with:

```c
struct zsl_fus_drv *drv = &madgwick_drv;

/* Init filter at 100 Hz. */
drv->init_handler(100, drv->config);

/* Start feeding the filter data. */
drv->feed_handler(&a, &m, &g, &q, drv->config);
```

To change to a different algorithm, simply change the `zsl_fus_drv` struct that
`*drv` points to.

## Credits

A big thanks to the [Python AHRS Library](https://ahrs.readthedocs.io/en/latest/index.html) for highlighting several less commonly-known fusion
algorithms, and the research papers behind them. Consider making a donation to
their great project via the [link on Github](https://github.com/Mayitzin/ahrs)
if you find what they've done useful!
