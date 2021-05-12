![zscilib](doc/img/zscilib_logo_200px.png)

# Zephyr Scientific Library (zscilib)

[![API Documentation](doc/img/badge_api.svg)][1]

[1]: https://zscilib.github.io/zscilib/

The **Zephyr Scientific Library (zscilib)** is an attempt to provide a set of
functions useful for scientific computing, data analysis and data manipulation
in the context of resource constrained embedded hardware devices.

It is written entirely in C, and while the main development target for the
library is the [Zephyr Project](https://github.com/zephyrproject-rtos/zephyr),
it tries to be as portable as possible, and a standalone reference project
is included to use this library in non-Zephyr-based projects.

This version of zscilib has been developed and tested against **Zephyr 2.5.0**.

## Quick Start: Zephyr

### Adding zscilib to your project via `west`

For project that have been setup using `west`, you can add a local copy of
zscilib by adding the following sections to `zephyr/west.yml`:

1. In the `manifest/remotes` section add:

```
remotes:
  - name: zscilib
    url-base: https://github.com/zscilib
```

2. In the `manifest/projects` section add:

```
- name: zscilib
  remote: zscilib
  path: modules/lib/zscilib
  revision: master
```

3. Save the file, and run `west update` from the project root to retrieve the
latest version of zscilib from Github, or whatever `revision` was specified
above.

### Running the benchmark sample

To run the benchmark sample using qemu, run the following commands:

> Be sure to run `source zephyr/zephyr-env.sh` (OS X or Linux) or
  `.\zephyr\zephyr-env.cmd` (Windows) before the commands below!

```
$ west build -p -b qemu_cortex_m3 modules/lib/zscilib/samples/benchmarking -t run
```

Press **`CTRL+A`** then **`x`** to quit qemu.

### Running Unit Tests

To run the unit tests for this library, run the following command:

```
$ twister -p qemu_cortex_m3 -T modules/lib/zscilib/tests
```

See the `tests` folder for further details.


### Debugging with QEMU

If you wish to debug using QEMU (and with minor variation actual hardware),
you can run the following commands to start a new GDB debug session.

> For an overview of debugging in GDB, you may find the following presentation
  useful: [LVC21-308: Essential ARM Cortex-M Debugging with GDB][LV21-308]

[LV21-308]: https://connect.linaro.org/resources/lvc21/lvc21-308/

In one terminal window, run:

```bash
$ west build -p auto -b qemu_cortex_m3 modules/lib/zscilib/samples/matrix/pinv
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

### Note: Float Stack Usage in Zephyr

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

## Quick Start: Standalone

A few makefile-based projects are included in `samples/standalone` showing
how zscilib can be used independent of Zephyr.

If you already have an appropriate GNU toolchain and build tools (`make`, etc.)
installed, you can simply execute the following commands:

```
$ cd samples/standalone/svd_pinv
$ make
$ bin/zscilib
  Hello, zscilib!
  ...
```

## Current Features

The feature tables below indicate whether implemented functions support:

- **f32**: Single-precision floating-point operations
- **f64**: Double-precision floating-point operations
- **ARM**: Optimised ARM Thumb-2 ASM implementation

### Vector Operations

| Feature         | Func                  | f32 | f64 | ARM | Notes           |
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
| Print           | `zsl_vec_print`       | x   | x   |     |                 |

### Matrix Operations

| Feature         | Func                  | f32 | f64 | ARM | Notes           |
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
| Multiply row (d)| `zsl_mtx_mult_row_d`  | x   | x   |     | Destructive     |
| Transpose       | `zsl_mtx_trans`       | x   | x   |     |                 |
| Adjoint         | `zsl_mtx_adjoint`     | x   | x   |     |                 |
| Reduce          | `zsl_mtx_reduce`      | x   | x   |     | Row+col removal |
| Reduce (iter)   | `zsl_mtx_reduce_iter` | x   | x   |     | Iterative ver.  |
| Augment         | `zsl_mtx_augm_diag`   | x   | x   |     | Adds row+col(s) |
| Determinant     | `zsl_mtx_deter`       | x   | x   |     |                 |
| Gaussian El.    | `zsl_mtx_gauss_elim`  | x   | x   |     |                 |
| Gaussian El. (d)| `zsl_mtx_gauss_elim_d`| x   | x   |     | Destructive     |
| Gaussian Rd.    | `zsl_mtx_gauss_reduc` | x   | x   |     |                 |
| Column norm.    | `zsl_mtx_cols_norm`   | x   | x   |     | Unitary col vals|
| Elem. norm.     | `zsl_mtx_norm_elem`   | x   | x   |     | Norm vals to i,j|
| Elem. norm. (d) | `zsl_mtx_norm_elem_d` | x   | x   |     | Destructive     |
| Gram-Schmidt    | `zsl_mtx_gram_schmidt`| x   | x   |     |                 |
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

#### Unary matrix operations

The following component-wise unary operations can be executed on a matrix
using the `zsl_mtx_unary_op` function:

- Increment (`++`)
- Decrement (`--`)
- Negative (`-`)
- Logical negation (`!`)
- Round
- Abs
- Floor
- Ceiling
- Exponent
- Natural log
- Log10
- Square root
- Sin, cos, tan
- Asin, acos, atan
- Sinh, cosh, tanh

#### Binary matrix operations

The following component-wise binary operations can be executed on a pair
of symmetric matrices using the `zsl_mtx_binary_op` function:

- Add (`a + b`)
- Subtract (`a - b`)
- Multiply (`a * b`)
- Divide (`a / b`)
- Mean (`mean(a, b`)
- Exponent (`a^b`)
- Min (`min(a, b)`)
- Max (`max(a, b)`)
- Equal (`a == b`)
- Not equal (`a != b`)
- Less than (`a < b`)
- Greater than (`a > b`)
- Less than or equal to (`a <= b`)
- Greater than or equal to (`a >= b`)

> **NOTE**: Component-wise **unary** and **binary** matrix operations can also
  make use of user-defined functions at the application level if the existing
  operand list is not sufficient. See `zsl_mtx_unary_func` and
  `zsl_mtx_binary_func` for details.

### Interpolation

- Nearest neighbour (AKA 'piecewise constant')
- Linear (AKA 'piecewise linear')
- Natural cubic spline

### Physics

#### Kinematics

- Change in distance (initial velocity, time, acceleration)
- Change in time (initial and final velocity, acceleration)
- Instantaneous velocity (initial velocity, time, acceleration)
- Velocity (initial velocity, distance, acceleration)
- Average velocity (distance, time)
- Acceleration (initial and final velocity, time)
- Centripetal acceleration (radius, period, via radius/speed or radius/period)

#### Projectiles

- Horizontal and vertical velocity components (initial velocity, theta)
- Total time of flight
  - Formula 1: gravity, y2, y1, Vy
  - Formula 2: initial and final vertical velocity and gravity
- Vertical motion: position at time (Vy, gravity, time, initial height)
- Horizontal motion: horizontal change of distance at time
- Velocity (overall velocity from vertical and horizontal components)
- Theta (and between vertical and horizontal velocity)
- Range (distance travelled from ground using initial velocity, gravity, angle)

#### Dynamics

- Newton's second law
- Mass-acceleration relationship
- Friction (Fn, uK/s)
- Normal force on an incline (in newtons based on mass, gravity, angle)

#### Work

- Work done over an interval of distance and constant applied force
- Work done cosine (as above but with x-component or on an incline)
- Work done sine (as above but with y-component or on an incline)
- Work-KE theorem

#### Energy

- Kinetic energy
- Elastic potential energy
- Gravitational potential energy
- Power (work/energy over time)
- Energy lost to friction
- Energy of a photon
- Mechanical energy of a system
- Total energy of a system

#### Momentum

- Calculate momentum (mass, velocity)
- Impulse (force, time)
- Change in momentum/force (mass, initial and final velocity)
- Elastic collision (when two objects collide and bounce)
- Inelastic collision (when two objects collide and stick)

#### Gravitation

- Orbital period
- Escape velocity
- Gravitational acceleration
- Orbital velocity
- Gravitational force
- Gravitational potential energy

#### Rotation

- Change in theta (analog to distance in kinematics)
- Change in time (analog to time in kinematics)
- Instantaneous angular velocity
- Angular velocity
- Average angular velocity
- Angular acceleration
- Rotational kinetic energy
- Power (torque multiplied by angular velocity)

#### Waves

TBD

#### Sound

- Pressure amplitude
- Decibels (sound level between two intensities of the same frequency)
- Intensity (pressure amplitude, bulk modulus, density)
- Shock wave angle (speed of sound and velocity through medium)
- Doppler effect
- Beats (frequency resulting from overlap of two similar frequencies)

#### Gases

- Kinetic theory of gases
- Ideal gas law
- Combined gas law (relationship of pressure, volume, temperature)
- Boyle's law (relationship of pressure, volume)
- Charles/Gay-Lussac law (relationship of pressure, volume)

#### Fluids

- Density (of substance in Kg/m^2)
- Simple pressure (force, area)
- Pressure in a fluid (at a certain height/depth, gravity, density, surf. pres.)
- Fluid flow rate proportion
- Fluid force rate proportion
- Bernoulli's equation
  - To known pressure
  - Calculate pressure and equate
- Volume flow rate

#### Thermodynamics

- Temperature conversion (fahrenheit, celsius, kelvin)
- Latent heat of fusion/vaporisation
- Heat (in joules of a material based on mass, specific heat and delta temp)
- Linear expansion of metal (length, alpha constant, change in temperature)
- Average velocity of molecules (RMS velocity of molecules in a gas)
- Mean free path
- Efficiency of a heat engine (based on energy of hot and cold chambers)
- Carnot engine proportion

#### Center of Mass

- Calculate the CoM of a group of objects based on their mass and distance from
  an arbitrary point

#### Electric

- Coulomb's law
- Charge density
- Potential energy
- Electric field
- Coulombs potential
- Electric flux
- Force from a charge

#### Electricity

- Current (charge per second)
- Resistors in series/parallel
- Capacitors in series/parallel
- Resistivity of wire
  - Include constants like aluminium, copper, steel, silicon, etc.
- Ohm's law
- Power
  - Current, voltage
  - Voltage, resistance
  - Current, resistance

#### Electrical Components

- Capacitance
  - Charge, voltage
  - Area, distance
- Energy stored in capacitor
- Energy stored in inductor
- Transformer turns to voltage
- Resistor/inductor/capacitor voltage relationship
- Resistor/capacitor charge/discharge
  - Current during charge
  - Current during discharge
  - Charge (in coulombs) during charge
  - Charge (in coulombs) during discharge
- Inductor/capacitor energising/de-energising
  - Energising
  - De-energising

#### Magnetics

- Magnetic force
- Force on current carrying wire
- Torque on current loop
- Potential energy from a dipole
- Orbital radius in magnetic field
- Magnetic flux
- Magnetic moment

#### Optics

- TBD

#### Relativity

- Time dilatation
- Lorentz contraction
- Relativistic momentum
- Kinetic energy
- Mass to energy
- Lorenz velocity transformation
- Relativistic doppler affect

#### Atomic

- Nuclear radius
- Radioactive decay
- Bohr orbital radius
- Bohr orbital velocity
- Bohr orbital energy
- Bragg's law

### Chemistry

- Periodic table data including:
  - Full name
  - Abbreviation
  - Atomic number
  - Standard atomic weight

## Planned Features

Help is welcome on the following planned or desirable features.

### Numerical Analysis

#### Scalar Operations

- Fast trigonometry approximations

#### Statistics Operations

- Mean
- Median
- Quantile
- Quartile
- Mode
- Data range
- De-mean
- Variance
- Standard deviation
- Interquartile range
- Covariance
- Covariance Matrix
- Correlation
- Error

#### Probability Operations

- Uniform probability density function (PDF)
- Uniform cumulative distribution function (CDF)
- Normal probability density function
- Normal cumulative distribution function
- Inverse normal cumulative distribution function
- Information entropy

### Digital Signal Processing

- Simple moving average filter
- Windowed moving average filter
- Weighted moving average filter
- Other basic IIR and FIR-type filters and helper functions.

### Machine Learning

#### Neural Networks

- Basic neural network processing
- Simplistic training of models
- Feeding data through a trained network

### Misc. Domain-Specific Operations

#### Motion and Orientation

- Acceleration/magnetic field -> orientation
- Sensor fusion (accel/mag/gyro -> quaternion)
- Euler/Quaternion conversion
- Functions for acceleration, time/distance, etc. (see `Physics` above)
- Frame of reference conversion (Aerospace, Android, etc.)

#### Spectrometry

- Conversion between radiometric and photometric units
- Radiometric data to lux
- Radiometric data to CCT/Duv
- Spectral analysis

#### Misc.

- Percent error (statistics?)
- Efficiency
- Quadratic formula

## Motivation

As the processing power of small, embedded MCUs increases and costs fall, more
computation can be done on the endnode itself. This allows for more of the
'complex' data analysis that used to take place at the PC or server level
(data aggregation, statistical analysis, etc.) to be done in less time,
using less data storage, and at a lower overall processing cost.

> A key goal of zscilib is to allow more data processing to happen on the
  endnode.

By generating immediately actionable and scientifically-relevant data points
(standard SI units, pre-filtered data, etc.) directly on the endnode, zscilib
aims to be a bridge between raw data and more numerically complex toolkits like
`gsl`, `numpy` or `R`.

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
  raw sensor data into actionable information (CMSIS-DAP, Lis).

The second item is of particular importance, since the goal of embedded systems
is often 'sensing' via raw data, correlating that data, and acting on the final
data points or passing them on for further analysis.

CMSIS-DAP contains a number of highly efficient algorithms for filtering
raw sensor data, but it doesn't offer any domain-specific assistance converting
filtered accelerometer vectors into orientation data, for example, or reading
a set of photodiodes and converting that data into a useful photometric value
like lux. It is excellent at 'conditioning' data, but not at 'understanding' it.

zscilib aims to find a middle ground between these two, allowing for richer
processing of raw data, but within the confines and limitations of the class
of microcontrollers commonly used on low-cost sensor endnodes.

## Architecture-Specific Optimisations

Basic tooling has been added to allow for optimised architecture-specific
implementations of key functions in assembly.

At present, this feature isn't being actively used or developed, but an aim
of zscilib is to add optimised versions of key functions to try to get the
best possible performance out of limited resources.

Initial optimisation will target the **ARM Cortex-M** family of devices and the
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

1. Fork the repository: https://github.com/zscilib/zscilib/fork
2. `git clone` your forked repository.
3. Update your local repo and commit any changes.
4. Push the changes out to your fork on Github.
5. Navigate to https://github.com/zscilib/zscilib and to the
   right of the **Branch** menu click **New pull request**.
6. Fill out the form that is presented.
7. Click the **Create Pull Request** button to submit the PR.

Also have a look at the [Issues][4] page to see if there is any outstanding
work or issues that you might be able to help with!

[4]: https://github.com/zscilib/zscilib/issues

## License

Apache 2.0.
