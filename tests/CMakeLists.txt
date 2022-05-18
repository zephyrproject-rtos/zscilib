cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr)
project(zsl)

zephyr_include_directories(include)

FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE ${app_sources})
