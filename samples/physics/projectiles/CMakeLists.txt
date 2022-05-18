cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr)
project(projectiles)

target_sources(app PRIVATE src/main.c)
