# zscilib Datatypes

The 'datatype' system is an attempt to encode any standard measurement type,
and it's underlying SI unit representation in a comprehensive, but compact
manner.

It encodes measurement descriptions into two components:

- **Measurement type** (`zsl_dt_meas_type`): which consist of two 8-bit
  values:
  - **Base type** (`zsl_dt_meas_base`): which identifies measurement families.
  - **Extended type**: which optionally specialises on the base type.
- **Unit** (`zsl_dt_unit_type`): The SI unit used to represent the measurement
  type, which consists of a:
  - **unit identifier** (`zsl_dt_unit_si`): A 16-bit value specifying which
    base, derived or combined SI unit type is used.
  - **CType identifier** (`zsl_dt_unit_ctype`): An 8-bit value which indicates
    how the unit is represented in memory.
  - **Scale factor**: An optional 8-bit signed value to indicate that the
    measurement value has been scaled up or down 10^n powers from the base SI
    unit's type and scale.

## Example

- Measurement Type:
  - Base: ZSL_DT_MEAS_BASE_LIGHT
  - Extended: ZSL_DT_MEAS_EXT_LIGHT_PHOTO_ILLUMINANCE
- Unit:
  - Unit: ZSL_DT_UNIT_SI_LUMEN_PER_METER_2
  - CType: ZSL_DT_UNIT_CTYPE_FLOAT32
  - Scale: ZSL_DT_UNIT_SCALE_NONE

This tells us that the measurement is for 'Illuminance', which is a photometric
value measured in lm/m^2, and represented using a single-precision (32-bit)
floating point value.

If we wished to represent the same measurement in `lm/mm^2` we could simply
add `ZSL_DT_UNIT_SCALE_MILLI` (-3) to the scale field.

## Work in Progress

This system is a work in progress, and the values, names and groups will see
regular changes until an initial, versioned release of zscilib happens.
