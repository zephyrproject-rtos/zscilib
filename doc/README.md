# Zephyr Scientific Library Documentation

## Getting Started

- [Add functions to zscilib](adding_funcs.md)

## API Documentation

API documentation is based on inline [doxygen](http://www.doxygen.nl/)
comment blocks.

To build the documentation, make sure you have a recent version of
doxygen installed and run the following command from the command
line:

```
$ doxygen doc/Doxyfile
```

This will produce an HTML version of the API documentation, with the
root page at `doxygen/html/index.html`.

## Platform-Specific Optimisation

### ARM Thumb/Thumb-2

- [ARM Assembly Primer](arm_asm_primer.md)
- [Inline ARM Assembly with GNU](arm_asm_gnu_inline.md)
- [Adding ARM ASM Functions to zscilib](arm_asm_adding.md)
