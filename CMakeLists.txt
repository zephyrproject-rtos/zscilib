zephyr_include_directories(include)

zephyr_sources(                                        src/zsl.c)
zephyr_sources(                                        src/matrices.c)
zephyr_sources(                                        src/vectors.c)
#zephyr_sources_ifdef(CONFIG_ZSL_SINGLE_PRECISION      src/zsl_todo.c)
