if(CONFIG_ZSL)
zephyr_interface_library_named(ZSCILIB)

zephyr_include_directories(include)

zephyr_library()
zephyr_library_sources(
    src/colorimetry/colorimetry.c
    src/colorimetry/conv.c
    src/colorimetry/illuminants.c
    src/colorimetry/lumeff.c
    src/colorimetry/norm.c
    src/colorimetry/observers.c
    src/colorimetry/rgbccms.c
    src/colorimetry/shell.c
    src/orientation/ahrs.c
    src/orientation/compass.c
    src/orientation/euler.c
    src/orientation/quaternions.c
    src/orientation/gravity.c
    src/orientation/shell.c
    src/orientation/fusion/madgwick.c
    src/orientation/fusion/mahony.c
    src/orientation/fusion/saam.c
    src/orientation/fusion/aqua.c
    src/orientation/fusion/complementary.c
    src/orientation/fusion/kalman.c
    src/orientation/fusion/calibration.c
    src/physics/atomic.c
    src/physics/dynamics.c
    src/physics/eleccomp.c
    src/physics/electricity.c
    src/physics/electric.c
    src/physics/energy.c
    src/physics/fluids.c
    src/physics/gases.c
    src/physics/gravitation.c
    src/physics/kinematics.c
    src/physics/magnetics.c
    src/physics/mass.c
    src/physics/misc.c
    src/physics/momentum.c
    src/physics/optics.c
    src/physics/photons.c
    src/physics/projectiles.c
    src/physics/relativity.c
    src/physics/rotation.c
    src/physics/sound.c
    src/physics/thermo.c
    src/physics/waves.c
    src/physics/work.c
    src/chemistry.c
    src/interp.c
    src/matrices.c
    src/probability.c
    src/shell.c
    src/statistics.c
    src/vectors.c
    src/zsl.c
)
#zephyr_library_sources_ifdef(CONFIG_ZSL_SINGLE_PRECISION src/zsl_todo.c)

zephyr_library_compile_options_ifdef(CONFIG_ZSL_SINGLE_PRECISION $<$<STREQUAL:${CMAKE_C_COMPILER_ID},GNU>:-fsingle-precision-constant>)
zephyr_library_compile_options_ifdef(CONFIG_ZSL_SINGLE_PRECISION $<$<STREQUAL:${CMAKE_C_COMPILER_ID},ARMClang>:-cl-single-precision-constant>)

zephyr_library_link_libraries(ZSCILIB)
target_link_libraries(ZSCILIB INTERFACE zephyr_interface)
endif()
