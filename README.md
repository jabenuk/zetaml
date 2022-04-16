# zetamlc: the Zeta Maths Library for C

## Documentation
You can see the official documentation [here](https://jabenuk.github.io/zetamlc/). The home page of that documentation is essentially mirrored in this README file.

## Summary
zetamlc is a C port of the original [Zeta Maths Library](https://github.com/jabenuk/zetaml) for C++.

Zetaml is suitable for any C/C++ project that requires more complex mathematical functions and objects such as matrices, vectors, etc. Keep in mind that I am developing this project mainly for my own use, so the features here are really just the features I need for my other project(s). This is a personal project, not a professional one. But I still try to put effort into its documentation. 

## Usage

Zetaml is built with [CMake](https://cmake.org/).

When compiling, use the `-DZML_USE_FLOATS` flag to use floats (32-bit floating values) instead of doubles (64-bit floating values). You can also use the `-DZML_BUILD_TESTS` flag to build test executable(s); this can be useful if you intend to help develop zetaml. Furthermore, you can use the `i386-linux-gnu.cmake toolchain` file to build for 32-bit with GCC - as zetaml aims to be as compatible as possible with early architectures, I recommend testing the project on both x86 and x86_64 architectures if you contribute at all. *As a sidenote: if you do decide to contribute, please remember to test your contributions for memory leaks with [Valgrind](https://valgrind.org/).*

To use the library, include `<zetamlc.h>`. 

## Configuring zetaml

You can use the `zmlSetLibFlag()` function to configure the library. All flags are false by default!

| Flag | Description | Additional notes|
| - | - | - |
| `ZML_USE_DEGREES` | Use degrees instead of radians. | Radians are used by default. |
| `ZML_USE_LEFT_COORDS` | Use left-handed coordinates instead of the default right-handed coordinates. | By convention, OpenGL uses right-handed coordinates, and DirectX uses left-handed coordinates. Zetaml uses right-handed by default. |

## Naming scheme of operator functions

Zetaml operators are named systematically, and all work in order of left-to-right (i.e. `zmlDivideVecs_r(v1, v2)` is `v1 / v2`).

For example, `zmlAddVecs_r` adds two vectors and returns the result, allocating heap space for the resulting vector. The '\_r' comes from the fact that the function **r**eturns its result. Therefore, \_r operators are equivalent to '+', '-', '\*', etc.

However, `zmlAddVecs` adds v2 (its second argument) to v1 (its first argument), modifying v1. There is no '\_r' because nothing is returned. Therefore, \_r operators are equivalent to '+=', '-=', '\*=', etc.

When different types are operated on, they are all included in the function name, e.g. `zmlMultiplyVecMat()`.

Finally, logical operators are named based on the initials of each syllable, e.g. `zmlVecLT` is a **L**ess **T**han operator and `zmlMatGTE` is a **G**reater **T**han or **E**qual to operator. 

##### See more on the [official documentation](https://jabenuk.github.io/zetamlc/).
