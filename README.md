<p align="center">
  <img src="/.github/icon/readme-icon-black.svg#gh-light-mode-only" height=80px/>
  <img src="/.github/icon/readme-icon-white.svg#gh-dark-mode-only" height=80px/>
</p>

# zetamlc: the Zeta Maths Library for C

_This readme is **unfinished**. It will be added to as development goes on._

## Summary
zetamlc is a C port of the original [Zeta Maths Library](https://github.com/jabenuk/zetaml) for C++.

zetamlc is suitable for any C/C++ project that requires more complex math functions and objects such as matrices, vectors, etc.

| :warning: Keep in mind: I'm designing this for my own use so it may be limited for uses outside of what I'm using it for (which is rendering with OpenGL). |
| - |

## Configuration and usage

When compiling, use the `-DZML_USE_FLOATS` to use **floats** (32-bit floating values) instead of **doubles** (64-bit floating values).

Some more configuration can be done by using the `zmlSetLibFlag()` function. The possible flags are listed below:
 - `ZML_USE_DEGREES`: use degrees instead of radians.
 - `ZML_USE_LEFT_COORDS`: use left-handed coordinates instead of right-handed coordinates.

These are boolean values, and all default to `0`.