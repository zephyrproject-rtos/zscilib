# Copyright (c) 2022 Linaro
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(zsl)

zephyr_include_directories(include)

FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE ${app_sources})

if (CONFIG_ZSL_SINGLE_PRECISION)
    target_compile_options(app PRIVATE $<$<STREQUAL:${CMAKE_C_COMPILER_ID},GNU>:-fsingle-precision-constant>)
    target_compile_options(app PRIVATE $<$<STREQUAL:${CMAKE_C_COMPILER_ID},ARMClang>:-cl-single-precision-constant>)
endif()
