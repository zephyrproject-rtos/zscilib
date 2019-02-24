# Zephyr Scientific Library (zscilib)

The **Zephyr scientific library (zscilib)** is an attempt to provide a set of functions
useful for scientific computing, data analysis and data manipulation in the
context of resource constrained embedded devices or 'end nodes'.

## Quick Start

### Adding zscilib to your Zephyr project

**TODO**: Setup Instructions based on `west`.

### Running Unit Tests

To run the unit tests for this library, run the following command:

```
$ sanitycheck -p qemu_cortex_m3 -T ext/lib/zsl/tests/
```

See the `tests` folder for further details.

## Motivation

As the cost of small, embedded MCUs increases and costs fall, more processing
can be done on the endnode itself, allowing the 'complex' off-node computing
(data aggregation, complex statistical analysis, etc.) to be done in less time,
using less data storage, and at a lower overall processing cost.

> **A key goal of zscilib is to allow more data processing to happen on the
  endnode**.

By generating immediately actionable and scientifically-relevant data points
(standard SI units, pre-filtered data, etc.) directly on the endnode, zscilib
aims to be a bridge between raw data and more numerically complex toolkits like
`gsl`, `numpy` or `R`.

#### Why Yet Another Library?

Numerous high quality, mature, open source scientific libraries already exist:

- [GNU scientific library (gsl)](https://www.gnu.org/software/gsl/)
- [Lis (Library of Iterative Solvers for linear systems)](https://github.com/anishida/lis)
- [CMSIS-DSP](http://www.keil.com/pack/doc/CMSIS/DSP/html/index.html)


Despite the wealth of mature functions in these existing libraries, though,
they tend to have the following two problems in an embedded context:

- They are overly broad and resource intensive (GSL, etc.), and this not
  appropriate for resource constrained devices like the ARM Cortex M family.
- They are missing many of the domain-specific features required to convert
  raw sensor data into actionable information (CMSIS-DAP, Lis).

The second item is of particular importance, since the goal of embedded systems
is often 'sensing' via raw data, manipulating that data, then acting on the
converted data points or passing them on for further analysis.

CMSIS-DAP contains a number of highly efficient algorithms for filtering
raw sensor data, but it doesn't offer any domain-specific assistance converting
filtered accelerometer vectors into orientation data, for example, or reading
a set of photodiodes and converting that data into a useful photometric value
like lux. It is excellent at 'conditioning' data, but not at 'understanding' it.

zscilib aims to find a middle ground between these two, allow for richer
processing of raw data, but within the confines and limitations of the class
of microcontrollers commonly used on low-cost sensor endnodes.

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
- Mean (two or more vectors)
- Reverse
- Equality check
- Not negative check

### Matrix Operations

- Add
- Subtract
- Multiply
- Scalar multiply
- Transpose
- Invert
- Get/set row/column vector
- Min/max value or index
- Minor
- Determinant
- Cofactor
- Adjoint
- Eigenvalues and eigenvectors

## Planned Features

The Zephyr scientific library is still in very early planning, but the
features listed below have been defined as key goals.

> Any suggested additions to this list are highly welcome at this early stage.

### Numerical Analysis

#### Matrix Operations

- Singular value decomposition (SVD)
- Pseudoinverse (AKA Moore-Penrose inverse)

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

### Calibration Management

- A framework to persist, retrieve and apply calibration coefficients.

### Domain-Specific Operations

#### Motion and Orientation

- Acceleration/magnetic field -> orientation
- Sensor fusion (accel/mag/gyro -> quaternion)
- Euler/Quaternion conversion
- Functions for acceleration, time/distance, etc.

#### Spectrometry

- Conversion between radiometric and photometric units
- Radiometric data to lux
- Spectral analysis

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

TODO: Note on how to contribute, and what it most needed at present.

## License

Apache 2.0.
