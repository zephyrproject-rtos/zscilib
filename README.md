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
$ rm -rf build
$ west build -b qemu_cortex_m3 modules/lib/zscilib/samples/benchmarking -t run
```

Press **`CTRL+A`** then **`x`** to quit qemu.

### Running Unit Tests

To run the unit tests for this library, run the following command:

```
$ sanitycheck -p qemu_cortex_m3 -T tests
```

See the `tests` folder for further details.

### Note: Float Stack Usage in Zephyr

The sample code in this library typically has the `CONFIG_FLOAT` option set,
meaning that floating-point support is configured for
[Unshared FP registers mode][2]. This mode is used when the application
has a **single thread** that uses floating point registers.

If your application makes use of **multiple threads**, and more than one of
these threads uses floating-point operations, you should also enable the
`CONFIG_FP_SHARING` config flag, which configures the kernel for
[Shared FP registers mode][3]. In this mode, the floating point registers are
saved and restored during each context switch, even when the associated threads
are not using them. This feature comes at the expense of an extra 72 bytes of
stack memory per stack frame (`s0..s15` + `FPCSR`, plus an alignment word
to ensure that the stack pointer is double-word aligned).

[2]: https://github.com/zephyrproject-rtos/zephyr/blob/master/doc/reference/kernel/other/float.rst#unshared-fp-registers-mode
[3]: https://github.com/zephyrproject-rtos/zephyr/blob/master/doc/reference/kernel/other/float.rst#shared-fp-registers-mode

## Quick Start: Standalone

A standard makefile-based project is included in `samples/standalone` showing
how zscilib can be built by itself with GCC (without using Zephyr).

If you already have an appropriate GNU toolchain and build tools (`make`, etc.) installed, you can simply execute the following commands:

```
$ cd samples/standalone
$ make
  Compiling obj/main.o
  Compiling obj/matrices.o
  Compiling obj/vectors.o
  Compiling obj/zsl.o
  gcc obj/main.o obj/matrices.o obj/vectors.o obj/zsl.o  -o bin/zscilib -Wall -I. -I../../include -lm
$ bin/zscilib
  Hello, zscilib!
  ...
```

## Current Features

### Vector Operations

- Add
- Subtract
- Negate
- Sum (two or more vectors)
- Magnitude
- Scalar add
- Scalar multiply
- Scalar divide
- Distance (between two vectors)
- Norm/absolute value
- To Unit Vector
- Dot product
- Cross product
- Sum of squares
- Component-wise mean (two or more vectors)
- Arithmetic mean (single vector)
- Reverse
- Equality check (size and values)
- Not-negative check
- Contains (occurrences of 'x')

### Matrix Operations

- Copy
- Add
- Subtract
- Multiply
- Scalar multiply
- Transpose
- Adjoint (3x3 and nxn)
- Determinant (3x3 and nxn)
- Inverse (3x3 and nxn)
- Eigenvectors/values
- Gauss-Jordan elimination
- Singular value decomposition (SVD)
- Pseudoinverse (AKA Moore-Penrose inverse)
- Get/set row/column vector
- Min/max value or index
- Equality test (shape and values)
- Not-negative check

#### Unary matrix operations

The following component-wise unary operations can be executed on a matrix:

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
of symmetric matrices:

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

## Planned Features

The Zephyr scientific library is still in very early planning, but the
features listed below have been defined as key future goals.

Contributions to this project can take the form of any of the following
requirements around these planned features and functions:

- Function implementation in C
- Optimised function implementation (ARM ASM, etc.) where appropriate
- Unit tests to ensure reliable execution
- Meaningful documentation (doxygen and `doc/topic.md` as appropriate)
- Sample project(s) showing how these functions can be used in the real world


> Any suggested additions to this list are highly welcome at this early stage.

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

#### Misc.

TODO: Where to classify these?

- Percent error (statistics?)
- Efficiency
- Quadratic formula

### Chemistry

- Periodic table data including:
  - Full name
  - Abbreviation
  - Atomic number (done)
  - Standard atomic weight (done)
  - TBD

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
