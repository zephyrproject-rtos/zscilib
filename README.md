![zscilib](doc/img/zscilib_logo_200px.png)

# Zephyr Scientific Library (zscilib)

[![API Documentation](doc/img/badge_api.svg)][1]

[1]: https://zephyrproject-rtos.github.io/zscilib/

The **Zephyr Scientific Library (zscilib)** is an attempt to provide a set of
functions useful for scientific computing, data analysis and data manipulation
in the context of resource constrained embedded hardware devices.

It is written entirely in C, and while the main development target for the
library is the [Zephyr Project](https://github.com/zephyrproject-rtos/zephyr),
it tries to be as portable as possible, and a standalone reference project
is included to use this library in non-Zephyr-based projects.

This version of zscilib has been developed and tested against **Zephyr 3.3.0**.

## Motivation

As the processing power of small, embedded MCUs increases and costs fall, more
computation can be done on the endnode itself. This allows for more of the
'complex' data analysis that used to take place at the PC or server level
(data aggregation, statistical analysis, etc.) to be done in less time,
using less data storage, and at a lower overall processing cost on small,
embedded devices.

> A key goal of zscilib is to allow more data processing to happen on the
  endnode.

By generating scientifically-relevant data points (standard SI units,
pre-filtered data, etc.) directly on the endnode, zscilib aims to be a bridge
between raw data and more numerically complex toolkits like `gsl`, `numpy`
or `R`.

#### What makes zscilib distinct?

Numerous high quality, mature, open source scientific libraries already exist:

- [GNU scientific library (gsl)](https://www.gnu.org/software/gsl/)
- [Lis (Library of Iterative Solvers for linear systems)](https://github.com/anishida/lis)
- [CMSIS-DSP](http://www.keil.com/pack/doc/CMSIS/DSP/html/index.html)

Despite the wealth of mature functions in these existing libraries, though,
they tend to have the following two problems in an embedded context:

- They are overly broad and resource intensive (GSL, etc.), and thus aren't
  appropriate for small, resource constrained devices like the ARM Cortex M
  family.
- They are missing many of the domain-specific features required to convert
  raw sensor data into actionable information (CMSIS-DSP, Lis).

The second item is of particular importance, since the goal of embedded systems
is often 'sensing' via raw data, correlating that data, and acting on the final
data points or passing them on for further analysis.

CMSIS-DSP contains a number of highly efficient algorithms for filtering
raw sensor data, but it doesn't offer any domain-specific assistance converting
filtered accelerometer vectors into orientation data, for example, or reading
a set of photodiodes and converting that data into a useful photometric value
like lux. It is excellent at 'conditioning' data, but not at 'understanding' it.

zscilib aims to find a middle ground between these two, allowing for richer
processing of raw data, but within the confines and limitations of the class
of microcontrollers commonly used on low-cost sensor endnodes.

## Quick Start: Standalone

A few makefile-based projects are included in `samples/standalone` showing
how zscilib can be used independent of Zephyr.

If you already have an appropriate GNU toolchain and build tools (`make`, etc.)
installed, you can simply execute the following commands:

```bash
$ cd samples/standalone/svd_pinv
$ make
$ bin/zscilib
  Hello, zscilib!
  ...
```

## Quick Start: Zephyr RTOS

### Running a sample application

To run one of the sample applications using qemu, run the following commands:

> Be sure to run `source zephyr/zephyr-env.sh` (OS X or Linux) or
  `.\zephyr\zephyr-env.cmd` (Windows) before the commands below! This also
  assumes `qemu-system-arm` is available on your local system.

```
$ west build -p -b mps2/an521/cpu0 \
  modules/lib/zscilib/samples/matrix/mult -t run
...
*** Booting Zephyr OS build zephyr-v2.6.0-536-g89212a7fbf5f  ***
zscilib matrix mult demo

mtx multiply output (4x3 * 3x4 = 4x4):

14.000000 17.000000 20.000000 23.000000 
35.000000 44.000000 53.000000 62.000000 
56.000000 71.000000 86.000000 101.000000 
7.000000 9.000000 11.000000 13.000000 
```

Press **`CTRL+A`** then **`x`** to quit qemu.

### Running Unit Tests

To run the unit tests for this library, run the following command:

```bash
$ twister --inline-logs -p mps2/an521/cpu0 -T modules/lib/zscilib/tests
```

See the `tests` folder for further details.

To run compliance tests to make sure submitted code matches Zephyr PR
requirements, run this (updating `HEAD~2` for the number of commits to check,
or setting it to `origin/master..` to check everything):

```bash
$ ../../../zephyr/scripts/ci/check_compliance.py \
  -m Gitlint -m Identity -m Nits -m pylint -m checkpatch \
  -c HEAD~2..
```

### Debugging with QEMU

If you wish to debug using QEMU (and with minor variation actual hardware),
you can run the following commands to start a new GDB debug session.

> For an overview of debugging in GDB, you may find the following presentation
  useful: [LVC21-308: Essential ARM Cortex-M Debugging with GDB][LV21-308]

[LV21-308]: https://connect.linaro.org/resources/lvc21/lvc21-308/

In one terminal window, run:

```bash
$ west build -p auto -b mps2/an521/cpu0 modules/lib/zscilib/samples/matrix/pinv
```

Once the ELF file has been built, we can start a GDB server on the default
`1234` socket, and wait for a new connection via:

```bash
$ cd build
$ ninja debugserver
```

In a new terminal window, connect to the GDB server via:

```bash
$ cd <zephyr_path>
$ arm-none-eabi-gdb-py \
  --eval-command="target remote localhost:1234" \
  --se=build/zephyr/zephyr.elf
```

> The `-py` extension is optional, and makes use of a version of GDB from the
  ARM GNU toolchain releases that enables Python scripts to be used with your
  debug sessions. See the [LVC21-308][LV21-308] presentation at the top of this
  section for details.

From here, you can start debugging with the `(gdb)` prompt.

For example:

```
(gdb) b main
(gdb) c
Continuing.

Breakpoint 1, main () at modules/lib/zscilib/samples/matrix/pinv/src/main.c:70
70              printf("\n\nzscilib pseudo-inverse demo\n\n");
(gdb) n
72              pinv_demo();
(gdb) step
pinv_demo () at modules/lib/zscilib/samples/matrix/pinv/src/main.c:25
25              zsl_real_t vi[18 * 3] = {
(gdb) n
... 
(gdb) quit
```

## Floating-Point Usage

zscilib can be configured to make use of single-precision (32-bit) or
double-precision (64-bit) floating point values via the
`CONFIG_ZSL_SINGLE_PRECISION` flag, which will determine the size of
`zsl_real_t` used throughout the library. The default setting for this
flag is `n`, meaning **64-bit values are used by default**.

There is a tradeoff between the added range and precision that 64-bit
(double-precision) floating point values offer, and the memory and performance
gains of the smaller, less-precise but faster 32-bit (single-precision)
operations.

> Due to the reduced precision of single-precision values, certain complex
  functions in zscilib are **only available when double-precision is enabled**
  (PINV, SVD, etc.).

### Comparison

#### Single-Precision (32-bit) Floats

- Require 4 bytes of memory to store
- Have about **7 significant digits of precision**
- Have a range of about 1.E-36 to 1.E+36
- HW acceleration available on Cortex-M4F, Cortex-M7 and most Cortex-M33 MCUs.
- Generates smaller, more efficient code than double

#### Double-Precision (64-bit) Floats

- Requires 8 bytes of memory to store
- Have about **13 significant digits of precision**
- Have a range of about 1.E-303 to 1.E+303
- HW acceleration generally only available on large, Cortex-M7 MCUs
- Generates larger code, with more processing overhead per operation

### Float Stack Usage in Zephyr

The sample code in this library typically has the `CONFIG_FPU` option set,
meaning that floating-point support is configured for
[Unshared FP registers mode][2]. This mode is used when the application
has a **single thread** that uses floating point registers.

If your application makes use of **multiple threads**, and more than one of
these threads uses floating-point operations, you should also enable the
`CONFIG_FPU_SHARING` config flag, which configures the kernel for
[Shared FP registers mode][3]. In this mode, the floating point registers are
saved and restored during each context switch, even when the associated threads
are not using them. This feature comes at the expense of an extra 72 bytes of
stack memory per stack frame (`s0..s15` + `FPCSR`, plus an alignment word
to ensure that the stack pointer is double-word aligned).

[2]: https://github.com/zephyrproject-rtos/zephyr/blob/master/doc/reference/kernel/other/float.rst#unshared-fp-registers-mode
[3]: https://github.com/zephyrproject-rtos/zephyr/blob/master/doc/reference/kernel/other/float.rst#shared-fp-registers-mode

## Current Features

> Features marked with the
  [v0.2.0](https://github.com/zephyrproject-rtos/zscilib/projects/2) flag are in progress
  or planned as part of the current release cycle, and may be partially
  implemented or stubbed at present.
  [v0.3.0](https://github.com/zephyrproject-rtos/zscilib/projects/3) indicates features planned for that later release.

### Linear Algebra

#### Vector Operations

- **f32**: Single-precision floating-point operations
- **f64**: Double-precision floating-point operations
- **ARM**: Optimised Arm Thumb-2 ASM implementation

| Feature         | Func                  | f32 | f64 | Arm | Notes           |
|-----------------|-----------------------|-----|-----|-----|-----------------|
| Array to vector | `zsl_vec_from_arr`    | x   | x   |     |                 |
| Copy            | `zsl_vec_copy`        | x   | x   |     |                 |
| Get subset      | `zsl_vec_get_subset`  | x   | x   |     |                 |
| Add             | `zsl_vec_add`         | x   | x   |     |                 |
| Subtract        | `zsl_vec_sub`         | x   | x   |     |                 |
| Negate          | `zsl_vec_neg`         | x   | x   |     |                 |
| Sum             | `zsl_vec_sum`         | x   | x   |     | 2 or more vects |
| Scalar add      | `zsl_vec_scalar_add`  | x   | x   |     |                 |
| Scalar multiply | `zsl_vec_scalar_mult` | x   | x   |     |                 |
| Scalar divide   | `zsl_vec_scalar_div`  | x   | x   |     |                 |
| Distance        | `zsl_vec_dist`        | x   | x   |     | Between 2 vects |
| Dot product     | `zsl_vec_dot`         | x   | x   |     |                 |
| Norm/abs value  | `zsl_vec_norm`        | x   | x   |     |                 |
| Project         | `zsl_vec_project`     | x   | x   |     |                 |
| To unit vector  | `zsl_vec_to_unit`     | x   | x   |     |                 |
| Cross product   | `zsl_vec_cross`       | x   | x   |     |                 |
| Sum of squares  | `zsl_vec_sum_of_sqrs` | x   | x   |     |                 |
| Comp-wise mean  | `zsl_vec_mean`        | x   | x   |     |                 |
| Arithmetic mean | `zsl_vec_ar_mean`     | x   | x   |     |                 |
| Reverse         | `zsl_vec_rev`         | x   | x   |     |                 |
| Zero to end     | `zsl_vec_zte`         | x   | x   |     | 0 vals to end   |
| Equality check  | `zsl_vec_is_equal`    | x   | x   |     |                 |
| Non-neg check   | `zsl_vec_is_nonneg`   | x   | x   |     | All values >= 0 |
| Contains        | `zsl_vec_contains`    | x   | x   |     |                 |
| Quicksort       | `zsl_vec_sort`        | x   | x   |     |                 |
| Print           | `zsl_vec_print`       | x   | x   |     |                 |

#### Matrix Operations

- **f32**: Single-precision floating-point operations
- **f64**: Double-precision floating-point operations
- **ARM**: Optimised Arm Thumb-2 ASM implementation

| Feature         | Func                  | f32 | f64 | Arm | Notes           |
|-----------------|-----------------------|-----|-----|-----|-----------------|
| Array to matrix | `zsl_mtx_from_arr`    | x   | x   |     |                 |
| Copy            | `zsl_mtx_copy`        | x   | x   |     |                 |
| Get value       | `zsl_mtx_get`         | x   | x   |     |                 |
| Set value       | `zsl_mtx_set`         | x   | x   |     |                 |
| Get row         | `zsl_mtx_get_row`     | x   | x   |     |                 |
| Set row         | `zsl_mtx_set_row`     | x   | x   |     |                 |
| Get col         | `zsl_mtx_get_col`     | x   | x   |     |                 |
| Set col         | `zsl_mtx_set_col`     | x   | x   |     |                 |
| Add             | `zsl_mtx_add`         | x   | x   |     |                 |
| Add (d)         | `zsl_mtx_add_d`       | x   | x   |     | Destructive     |
| Sum rows        | `zsl_mtx_sum_rows_d`  | x   | x   |     | Destructive     |
| Sum rows scaled | `zsl_mtx_sum_rows_scaled_d` | x | x |   | Destructive     |
| Subtract        | `zsl_mtx_sub`         | x   | x   |     |                 |
| Subtract (d)    | `zsl_mtx_sub_d`       | x   | x   |     | Destructive     |
| Multiply        | `zsl_mtx_mult`        | x   | x   |     |                 |
| Multiply (d)    | `zsl_mtx_mult_d`      | x   | x   |     | Destructive     |
| Multiply sc (d) | `zsl_mtx_scalar_mult_d`|x   | x   |     | Destructive     |
| Multiple row sc (d)| `zsl_mtx_scalar_mult_row_d`|x|x|     | Destructive     |
| Transpose       | `zsl_mtx_trans`       | x   | x   |     |                 |
| Adjoint 3x3     | `zsl_mtx_adjoint_3x3` | x   | x   |     |                 |
| Adjoint         | `zsl_mtx_adjoint`     | x   | x   |     |                 |
| Wedge product   | `zsl_mtx_vec_wedge`   |     | x   |     |                 |
| Reduce          | `zsl_mtx_reduce`      | x   | x   |     | Row+col removal |
| Reduce (iter)   | `zsl_mtx_reduce_iter` | x   | x   |     | Iterative ver.  |
| Augment         | `zsl_mtx_augm_diag`   | x   | x   |     | Adds row+col(s) |
| Determinant 3x3 | `zsl_mtx_deter_3x3`   | x   | x   |     |                 |
| Determinant     | `zsl_mtx_deter`       | x   | x   |     |                 |
| Gaussian El.    | `zsl_mtx_gauss_elim`  | x   | x   |     |                 |
| Gaussian El. (d)| `zsl_mtx_gauss_elim_d`| x   | x   |     | Destructive     |
| Gaussian Rd.    | `zsl_mtx_gauss_reduc` | x   | x   |     |                 |
| Column norm.    | `zsl_mtx_cols_norm`   | x   | x   |     | Unitary col vals|
| Gram-Schimdt    | `zsl_mtx_gram_schmidt`| x   | x   |     |                 |
| Elem. norm.     | `zsl_mtx_norm_elem`   | x   | x   |     | Norm vals to i,j|
| Elem. norm. (d) | `zsl_mtx_norm_elem_d` | x   | x   |     | Destructive     |
| Invert 3x3      | `zsl_mtx_inv_3x3`     | x   | x   |     |                 |
| Invert          | `zsl_mtx_inv`         | x   | x   |     |                 |
| Balance         | `zsl_mtx_balance`     | x   | x   |     |                 |
| Householder Ref.| `zsl_mtx_householder` | x   | x   |     |                 |
| QR decomposition| `zsl_mtx_qrd`         | x   | x   |     |                 |
| QR decomp. iter.| `zsl_mtx_qrd_iter`    |     | x   |     |                 |
| Eigenvalues     | `zsl_mtx_eigenvalues` |     | x   |     |                 |
| Eigenvectors    | `zsl_mtx_eigenvectors`|     | x   |     |                 |
| SVD             | `zsl_mtx_svd`         |     | x   |     |                 |
| Pseudoinverse   | `zsl_mtx_pinv`        |     | x   |     |                 |
| Min value       | `zsl_mtx_min`         | x   | x   |     |                 |
| Max value       | `zsl_mtx_max`         | x   | x   |     |                 |
| Min index       | `zsl_mtx_min_idx`     | x   | x   |     |                 |
| Max index       | `zsl_mtx_max_idx`     | x   | x   |     |                 |
| Equality check  | `zsl_mtx_is_equal`    | x   | x   |     |                 |
| Non-neg check   | `zsl_mtx_is_notneg`   | x   | x   |     | All values >= 0 |
| Symmetr. check  | `zsl_mtx_is_sym`      | x   | x   |     |                 |
| Print           | `zsl_mtx_print`       | x   | x   |     |                 |

##### Unary matrix operations

The following component-wise unary operations can be executed on a matrix
using the `zsl_mtx_unary_op` function:

- [x] Increment (`++`)
- [x] Decrement (`--`)
- [x] Negative (`-`)
- [x] Logical negation (`!`)
- [x] Round
- [x] Abs
- [x] Floor
- [x] Ceiling
- [x] Exponent
- [x] Natural log
- [x] Log10
- [x] Square root
- [x] Sin, cos, tan
- [x] Asin, acos, atan
- [x] Sinh, cosh, tanh

##### Binary matrix operations

The following component-wise binary operations can be executed on a pair
of symmetric matrices using the `zsl_mtx_binary_op` function:

- [x] Add (`a + b`)
- [x] Subtract (`a - b`)
- [x] Multiply (`a * b`)
- [x] Divide (`a / b`)
- [x] Mean (`mean(a, b`)
- [x] Exponent (`a^b`)
- [x] Min (`min(a, b)`)
- [x] Max (`max(a, b)`)
- [x] Equal (`a == b`)
- [x] Not equal (`a != b`)
- [x] Less than (`a < b`)
- [x] Greater than (`a > b`)
- [x] Less than or equal to (`a <= b`)
- [x] Greater than or equal to (`a >= b`)

> **NOTE**: Component-wise **unary** and **binary** matrix operations can also
  make use of user-defined functions at the application level if the existing
  operand list is not sufficient. See `zsl_mtx_unary_func` and
  `zsl_mtx_binary_func` for details.

### Numerical Analysis

#### Statistics

- [x] Mean
- [x] Trimmed mean
- [x] Weighted mean
- [x] Time-weighted mean
- [x] De-mean
- [x] Percentile (AKA quantile)
- [x] Median
- [x] Weighted median
- [x] Quartile
- [x] Interquartile range
- [x] Mode
- [x] Data range
- [x] Mean absolute deviation
- [x] Median absolute deviation from median
- [x] Variance
- [x] Standard deviation
- [x] Covariance
- [x] Covariance matrix
- [x] Linear regression
- [x] Multiple linear regression \[1\]
- [x] Weighted multiple minear regression \[1\]
- [x] Quadrid fitting (Least-squars fitting of a quadric surface) \[1\]
- [x] Absolute error
- [x] Relative error
- [x] Standard error

\[1\] Only available in double-precision

#### Probability Operations

- [X] Uniform probability density function (PDF)
- [X] Uniform distribution mean
- [X] Uniform distribution variance
- [X] Uniform cumulative distribution function (CDF)
- [X] Normal probability density function (PDF)
- [X] Normal cumulative distribution function (CDF)
- [X] Inverse Error function
- [X] Inverse normal cumulative distribution function (CDF)
- [x] Factorial
- [X] Binomial coefficient
- [X] Binomial probability density function (PDF)
- [X] Binomial distribution mean
- [X] Binomial distribution variance
- [X] Binomial cumulative distribution function (CDF)
- [X] Information entropy
- [x] Bayes' Theorem

### Interpolation

- [x] Nearest neighbour (AKA 'piecewise constant')
- [x] Linear (AKA 'piecewise linear')
- [x] Natural cubic spline

### Physics

#### Atomic

- [x] Nuclear radius
- [x] Atomic Radioactive decay
- [x] Bohr orbital radius
- [x] Bohr orbital velocity
- [x] Bohr orbital energy
- [x] Bragg's law

#### Dynamics

- [x] Newton's second law
- [x] Mass-acceleration relationship
- [x] Friction (Fn, uK/s)
- [x] Normal force on an incline (in newtons based on mass, gravity, angle)
- [x] Tension
- [x] Dynamic lever
- [x] Pendulums
  - [x] Period
  - [x] Max Speed

#### Electrical Components

- [x] Capacitance
  - [x] Charge, voltage
  - [x] Area, distance
- [x] Energy stored in capacitor
- [x] Energy stored in inductor
- [x] Transformer turns to voltage
- [x] Resistor/inductor/capacitor voltage relationship
- [x] Resistor/capacitor charge/discharge
  - [x] Current during charge
  - [x] Current during discharge
  - [x] Charge (in coulombs) during charge
  - [x] Charge (in coulombs) during discharge
- [x] Current of RL circuit in time

#### Electric

- [x] Coulomb's law
- [x] Charge density
- [x] Potential energy
- [x] Electric field
- [x] Coulombs potential
- [x] Electric flux
- [x] Force from a charge

#### Electricity

- [x] Current (charge per second)
- [x] Resistors in series/parallel
- [x] Capacitors in series/parallel
- [x] Resistivity of wire
- [x] Ohm's law
- [x] Power
  - [x] Current, voltage
  - [x] Voltage, resistance
  - [x] Current, resistance

#### Energy

- [x] Kinetic energy
- [x] Elastic potential energy
- [x] Gravitational potential energy
- [x] Power (work/energy over time)
- [x] Energy lost to friction
- [x] Energy of a photon
- [x] Mechanical energy of a system
- [x] Total energy of a system

#### Fluids

- [x] Density (of substance in Kg/m^2)
- [x] Simple pressure (force, area)
- [x] Pressure in a fluid (at a certain height/depth, gravity, density, surf. pres.)
- [x] Bouyant Force
- [x] Fluid flow rate proportion
- [x] Fluid force rate proportion
- [x] Bernoulli's equation
- [x] Volume flow rate

#### Gases

- [x] Average velocity of a gas molecule (mass, temp, moles)
- [x] Ideal gas law (pressure based on moles, temp, volume)
- [x] Boyle's law (relationship of pressure, volume)
- [x] Charles/Gay-Lussac law (relationship of pressure, volume)

#### Gravitation

- [x] Orbital period
- [x] Escape velocity
- [x] Gravitational acceleration
- [x] Orbital velocity
- [x] Gravitational force
- [x] Gravitational potential energy

#### Kinematics

- [x] Change in distance (initial velocity, time, acceleration)
- [x] Initial position (final pos, init velocity, accel, time)
- [x] Initial position (final pos, init velocity, final velocity, accel)
- [x] Change in time (initial and final velocity, acceleration)
- [x] Instantaneous velocity (initial velocity, time, acceleration)
- [x] Velocity (initial velocity, distance, acceleration)
- [x] Average velocity (distance, time)
- [x] Acceleration (initial and final velocity, time)
- [x] Centripetal acceleration (radius, period, via radius/speed or radius/period)

#### Magnetics

- [x] Magnetic force
- [x] Force on current carrying wire
- [x] Torque on current loop
- [x] Potential energy from a dipole
- [x] Orbital radius in magnetic field
- [x] Magnetic flux
- [x] Magnetic moment

#### Mass

- [x] Calculate the CoM of a group of objects based on their mass and
      distance from an arbitrary point

#### Momentum

- [x] Calculate momentum (mass, velocity)
- [x] Impulse (force, time)
- [x] Change in momentum/force (mass, initial and final velocity)
- [x] Elastic collision (when two objects collide and bounce)
- [x] Inelastic collision (when two objects collide and stick)

#### Optics

- [x] Refraction index
- [x] Snell's law
- [x] Focus distance
- [x] Critical angle of incision
- [x] Power (based on focal length)
- [x] Magnification (real and apparent length)
- [x] Diffraction

#### Photons

- [x] Energy
- [x] Linear momentum
- [x] Wavelength
- [x] Frequency
- [x] Kinetic energy of photoelectric effect

#### Projectiles

- [x] Horizontal and vertical velocity components (initial velocity, theta)
- [x] Total time of flight
  - Formula 1: gravity, y2, y1, Vy
  - Formula 2: initial and final vertical velocity and gravity
- [x] Vertical motion: position at time (Vy, gravity, time, initial height)
- [x] Horizontal motion: horizontal change of distance at time
- [x] Velocity (overall velocity from vertical and horizontal components)
- [x] Trajectory
- [x] Theta (angle between vertical and horizontal velocity)
- [x] Range (distance travelled from ground using initial velocity, gravity, angle)

#### Relativity (v0.3.0)

- [ ] Time dilatation
- [ ] Lorentz contraction
- [ ] Relativistic momentum
- [ ] Kinetic energy
- [ ] Mass to energy
- [ ] Lorenz velocity transformation
- [ ] Relativistic doppler affect

#### Rotation

- [x] Change in theta (analog to distance in kinematics)
- [x] Distance travelled in circular motion
- [x] Number of rotations around circle
- [x] Change in time (analog to time in kinematics)
- [x] Instantaneous angular velocity
- [x] Angular velocity
- [x] Average angular velocity
- [x] Angular acceleration
- [x] Rotational kinetic energy
- [x] Rotational period
- [x] Rotational frequency
- [x] Centripetal acceleration
- [x] Total acceleration
- [x] Mechanical power (torque multiplied by angular velocity)

#### Sound

- [x] Pressure amplitude
- [x] Decibels (sound level between two intensities of the same frequency)
- [x] Intensity (pressure amplitude, bulk modulus, density)
- [x] Shock wave angle (speed of sound and velocity through medium)
- [x] Doppler effect
- [x] Beats (frequency resulting from overlap of two similar frequencies)

#### Thermodynamics

- [x] Temperature conversion (fahrenheit, celsius, kelvin)
- [x] Latent heat of fusion/vaporisation
- [x] Heat (in joules of a material based on mass, specific heat and delta temp)
- [x] Linear expansion of metal (length, alpha constant, change in temperature)
- [x] Mean free path
- [x] Efficiency of a heat engine (based on energy of hot and cold chambers)
- [x] Carnot engine proportion

#### Waves

- TBD

#### Work

- [x] Work done over an interval of distance and constant applied force
- [x] Work done cosine (as above but with x-component or on an incline)
- [x] Work done sine (as above but with y-component or on an incline)
- [x] Work-KE theorem

### Motion and Orientation

#### AHRS/Attitude (Degrees)

- [x] Basic struct definitions
- [x] Conversion
  - [x] To vector (access vector API)
  - [x] To Euler (degrees to radian)
  - [x] From Euler (radians to degrees)
  - [x] From Accel + Mag (roll, pitch, yaw)
  - [x] From Accel (roll, pitch)
- [x] Angle between two accelerometers

#### Compass

- [x] Degrees, Minutes, Seconds to Decimal Degrees (dms to dd)
- [x] Magnetic north
- [x] Geographic (AKA true) north

#### Euler Angles (Radians)

- [x] Basic struct definitions
- [x] Conversion
  - [x] To vector (access vector API)

#### Gravity

- [x] Gravitational field from latitude and altitude

#### Quaternions

- [x] Basic struct definitions
- [x] Magnitude
- [x] Unit check
- [x] Scaling
- [x] Multiplication
- [x] Exp
- [x] Log
- [x] Exponentiation
- [x] Conjugate
- [x] Inverse
- [x] Difference
- [x] Rotate
- [x] Interpolation
  - [x] Lerp
  - [x] Slerp
- [x] Integration
  - [x] Angular velocity (rad/s + time + current value)
  - [x] Angular momentum (Same as above, plus rotational mass)
- [x] Conversion
  - [x] To unit (Normalise)
  - [x] To Euler (radians)
  - [x] From Euler (radians)
  - [x] To rotation matrix
  - [x] From rotation matrix
  - [x] To axis-angle
  - [x] From axis-angle
- [x] Special Forms
  - [x] Identity

#### Sensor Fusion

- [x] Define generic fusion interface/struct (accel+mag+gyro -> quaternion)
- [x] Matrix rotation (adjust device orientation)
- [x] Axis-angle rotation (adjust device orientation)
- [x] Calibration
  - [x] Magnetometer (hard-iron and soft-iron errors)
  - [x] Correct magnetometer (apply correction coefficients)
  - [x] Madgwick calibration
  - [x] Mahoney calibration
- [x] Implementations
  - [x] AQUA
  - [x] Complementary
  - [x] Extended Kalman
  - [x] Madgwick
  - [x] Mahoney
  - [x] SAAM

### Colorimetry

#### Types/Structs

- [x] Spectral power distribution (SPD)
- [x] CIE 1931 XYZ tristimulus
- [x] CIE 1931 xyY chromaticity
- [x] CIE 1960 UCS chromaticity
- [x] CIE 1976 UCS chromaticity
- [x] RGBA color using floating-space notation (0.0 .. 1.0)
- [x] RGBA color using 8-bit values
- [x] RGBA color using 16-bit values
- [x] CIE 1960 CCT, Duv value pair
- [x] CIE 1931 XYZ tristimulus values for a standard illuminant
- [x] CIE 1931 XYZ tristimulus values for a standard observer model

#### Functions

- [x] SPD normalisation
- [x] SPD to XYZ tristimulus
- [x] CIE 1931 xyY chromaticity to XYZ tristimulus
- [x] CIE 1931 XYZ tristimulus values to xyY chromaticity
- [x] CIE 1931 xyY chromaticity to CIE 1960 uv
- [x] CIE 1931 XYZ tristimulus to CIE 1960 uv
- [x] CIE 1960 uv to CIE 1931 XYZ tristimulus
- [x] CIE 1960 uv to CIE 1931 xyY chromaticity
- [x] CIE 1960 uv to CIE 1976 u'v'
- [x] CIE 1976 u'v' value to CIE 1960 uv
- [x] Color temperature to (u,v) chromaticity
- [x] CIE 1960 CCT (Duv = 0.0) to CIE 1931 XYZ tristimulus
- [x] CIE 1960 CCT (Duv = 0.0) to 8-bit RGBA (supplied XYZ to RGB color space correlation matrix)
- [x] CIE 1960 CCT (Duv = 0.0) to float RGBA (supplied XYZ to RGB color space correlation matrix)
- [x] CIE 1960 CCT and Duv pair to CIE 1931 xyY chromaticity
- [x] CIE 1960 CCT and Duv pair to CIE 1931 XYZ tristimulus
- [x] CIE 1960 (u, v) pair to CIE 1960 CCT and Duv pair using:
  - [x] McCamy
  - [x] Ohno 2014
- [x] CIE 1931 XYZ tristimulus to 8-bit RGBA (supplied XYZ to RGB color space correlation matrix)
- [x] CIE 1931 XYZ tristimulus to float RGBA (supplied XYZ to RGB color space correlation matrix)
- [ ] Gamma encode
- [ ] Gamma decode

#### Color Data

##### Illuminants

- [x] A
- [x] B
- [x] C
- [x] D50
- [x] D55
- [x] D65
- [x] E
- [x] ICC

##### CIE Standard Observer Models

- [x] CIE 1931 2 degree standard observer color matching functions
- [x] CIE 1964 10 degree standard observer color matching functions

##### CIE Luminous Efficiency Functions

- [x] CIE 1988 Photopic
- [x] CIE 1951 Scotopic
- [x] CIE LERP interpolation helper function

##### XYZ to RGB Color Space Correlation Matrices

- [x] XYZ to linear sRGB (D65)
- [x] XYZ to linear sRGB (D50)
- [x] XYZ to Adobe RGB 98
- [x] XYZ to Sony S-Gamut3.cine D65
- [x] XYZ to NTSC
- [x] XYZ to PAL/SECAM
- [x] XYZ to ITU-R BT.709
- [x] XYZ to ITU-R BT.2020
- [x] XYZ to ACES Primaries #0 (AP0)
- [x] XYZ to ACES Primaries #1 (AP1)
- [x] XYZ to DCI-P3
- [x] XYZ to DCI-P3+
- [x] XYZ to CIE RGB

### Chemistry

- [ ] Periodic table data including:
  - [ ] Full name
  - [ ] Abbreviation
  - [x] Atomic number
  - [x] Standard atomic weight

### Measurement API (v0.2.0)

The `zsl_measurement` struct is a proof of concept attempt at representing
measurements in a concise but unambiguous manner.

It consists of:

- A **measurement type** (Base Type + Extended Type)
- The **SI unit** it uses (SI Unit Type)
- The specific **C type** used to represent it in memory (C Type)
- Additional meta-data to help interpret the measurement payload

There is an option to adjust the measurement's **scale** in +/- 10^n steps
(Scale Factor) from the default SI unit and scale indicated by the SI Unit
Type. For example, if 'Ampere' is indicated as the SI unit, the measurement
could indicate that the value is in uA by setting the scale factor to -6.

- [x] Measurement struct(s) (see: `zsl_measurement`)
- [x] Base Measurement Types
- [ ] Extended Measurement Types
  - [x] Color
  - [x] Light
  - [x] Temperature
  - [ ] Other groups TBD
- [x] SI Units
- [x] SI Scales
- [x] C Types

## Longer Term Planned Features

Help is welcome on the following planned or desirable features.

### Scalar Operations

- Fast trigonometry approximations

### Digital Signal Processing (v0.3.0)

- Simple moving average filter
- Windowed moving average filter
- Weighted moving average filter
- Other basic IIR and FIR-type filters and helper functions.

### Spectrometry

- Conversion between radiometric and photometric units
- Radiometric data to lux
- Radiometric data to CCT/Duv
- Spectral analysis

### Calibration

- Offset management
- Correlation matrix generation
- Non-linear compensation

## Architecture-Specific Optimisations

Basic tooling has been added to allow for optimised architecture-specific
implementations of key functions in assembly.

At present, this feature isn't being actively used or developed, but an aim
of zscilib is to add optimised versions of key functions to try to get the
best possible performance out of limited resources.

Initial optimisation will target the **Arm Cortex-M** family of devices and the
**Thumb** and **Thumb-2** instruction sets, though other architectures can be
accommodated if necessary or useful.

## Code Style

Since the primary target of this codebase is running as a module in
[Zephyr OS][CS0], it follows the same [coding style][CS1], which is itself
based on the [Linux kernel coding style][CS2].

You can format the source code to match this style automatically using the
[uncrustify][CS3] command line tool, which has plugins available for many
common text editors (Atom Beautify, for example).

- [Crustify rules file][CS4]

[CS0]: https://www.zephyrproject.org/
[CS1]: https://docs.zephyrproject.org/latest/contribute/index.html#coding-style
[CS2]: https://kernel.org/doc/html/latest/process/coding-style.html
[CS3]: http://uncrustify.sourceforge.net/
[CS4]: https://github.com/zephyrproject-rtos/zephyr/blob/master/.uncrustify.cfg

## Contributing

If you wish to contribute to this library, you can raise a PR as follows:

1. Fork the repository: https://github.com/zephyrproject-rtos/zscilib/fork
2. `git clone` your forked repository.
3. Update your local repo and commit any changes.
4. Push the changes out to your fork on Github.
5. Navigate to https://github.com/zephyrproject-rtos/zscilib and to the
   right of the **Branch** menu click **New pull request**.
6. Fill out the form that is presented.
7. Click the **Create Pull Request** button to submit the PR.

Also have a look at the [Issues][4] page to see if there is any outstanding
work or issues that you might be able to help with!

[4]: https://github.com/zephyrproject-rtos/zscilib/issues

## License

Apache 2.0.
