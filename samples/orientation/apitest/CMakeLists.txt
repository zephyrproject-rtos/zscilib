cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr)
project(orientation_api)

target_sources(app PRIVATE src/main.c)
target_sources(app PRIVATE src/data.c)
