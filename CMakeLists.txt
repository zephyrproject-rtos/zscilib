if(CONFIG_ZSL)
zephyr_interface_library_named(ZSCILIB)

zephyr_include_directories(include)

zephyr_library()
zephyr_library_sources(
    src/matrices.c
    src/vectors.c
    src/zsl.c
)
#zephyr_library_sources_ifdef(CONFIG_ZSL_SINGLE_PRECISION src/zsl_todo.c)

zephyr_library_link_libraries(ZSCILIB)
target_link_libraries(ZSCILIB INTERFACE zephyr_interface)
endif()
