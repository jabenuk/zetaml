# ----------------
# CMake toolchain file to build with x86 (i386) architecture. (Linux GCC/G++).

set(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_FLAGS -m32)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS -m32)

set(X86_BUILD ON)